#include "os/glfw_driver.h"
#include "compat/misc.h"
#include "compat/gl.h"
#include "main/game_platform.h"
#include "main/references.h"
#include "main/logging.h"
#include "threads.h"
#include "vec2.h"
#include <list>
#include <math.h>
#include <set>
#include <algorithm>

#ifndef WIN_MODE
#include <sys/time.h>
#include <random>
#else

// //Undef duplicate macros from glfw
// #undef APIENTRY
// #undef WINGDIAPI

#include <Windows.h>
#include <WinCrypt.h>
#include "os/resource.h"
#endif

//We use std::max instead
#undef max

#define	SR2_MOUSE_BUTTON_LEFT		0
#define SR2_MOUSE_BUTTON_RIGHT		1
#define SR2_MOUSE_BUTTON_MIDDLE		2

extern bool game_running;

namespace os {

void redirectWindowClose(SDL_Window*);
void redirectWindowResize(SDL_Window*,int, int);
void redirectSpecialKey(SDL_Window*,int key, int scan, int action, int mods);
void redirectCharKey(SDL_Window*,unsigned);
void redirectMouseButton(SDL_Window*,int button, int action, int mods);
void redirectMouseMove(SDL_Window*,double x, double y);
void redirectMouseWheel(SDL_Window*,double x, double y);
void trackMouseOver(SDL_Window*, int over);
void pollEvents(SDL_Window*);

class SDL2Driver;
SDL2Driver* driver = 0;

class SDL2Driver : public OSDriver {
	union Callback {
		std::function<bool(void)>* f_v;
		std::function<bool(int)>* f_i;
		std::function<bool(int,int)>* f_ii;

		Callback(decltype(f_v) p_f_v) : f_v(p_f_v) {}
		Callback(decltype(f_i) p_f_i) : f_i(p_f_i) {}
		Callback(decltype(f_ii) p_f_ii) : f_ii(p_f_ii) {}
	};

	std::list<Callback> callbacks[OSC_COUNT];
	SDL_Window* window;
	SDL_GLContext glcontext;

public:
#ifndef WIN_MODE
	timeval start_time;
#else
	ULARGE_INTEGER start_time;

	LARGE_INTEGER start_time_hq, start_time_hq_freq;
#endif

	bool mouseOver;
	bool canLock, shouldLock;

	double frameTime, gameTime, gameSpeed;

	SDL2Driver() : window(0), mouseOver(true), canLock(false), shouldLock(false) {
		//glfwSetErrorCallback(glfwError);
		//glfwInit();
		SDL_Init(SDL_INIT_VIDEO);
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
		resetTimer();
	}

	~SDL2Driver() {
		driver = 0;
		SDL_GL_DeleteContext(glcontext);  
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	bool systemRandom(unsigned char* buffer, unsigned bytes) override {
#ifdef WIN_MODE
		HCRYPTPROV provider;
		
		//Attempt to acquire the context if it exists, then make it if it does not
		BOOL ctx = CryptAcquireContext(&provider, "SR2SysRand", NULL, PROV_RSA_AES, NULL);
		if(!ctx)
			ctx = CryptAcquireContext(&provider, "SR2SysRand", NULL, PROV_RSA_AES, CRYPT_NEWKEYSET);

		if(ctx) {
			BOOL success = CryptGenRandom(provider, bytes, buffer);
			CryptReleaseContext(provider, NULL);

			return success != 0;
		}
		return false;
#else
		std::random_device rd;
		for(unsigned i = 0; i < bytes; ++i)
			buffer[i] = (unsigned char)rd();
		return true;
#endif
	}

	void setVerticalSync(int waitFrames) override {
		SDL_Log("setVerticalSync: %i", waitFrames);
		SDL_GL_SetSwapInterval(1);
	}

	void swapBuffers(double minWait_s) override {
		SDL_GL_SwapWindow(window);

		pollEvents(window);
		if(devices.cloud)
			devices.cloud->update();

		double waitTill = frameTime + minWait_s;
		double curTime = getAccurateTime();

		while(curTime < waitTill) {
			pollEvents(window);
			if(devices.cloud)
				devices.cloud->update();
			threads::sleep(0);
			curTime = getAccurateTime();
		}

		if(gameSpeed > 0 && game_running) {
			double delta = curTime - frameTime;
			if(delta > 0.5)
				delta = 0.5;
			gameTime += delta * gameSpeed;
		}

		frameTime = curTime;
	}

	void handleEvents(unsigned minWait_ms) override {
		pollEvents(window);

		frameTime = getAccurateTime();
		threads::sleep(minWait_ms);
	}

	void getDesktopSize(unsigned& width, unsigned& height) override {
		SDL_Rect r;
		if (SDL_GetDisplayBounds(0, &r) != 0) {
			SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());
		}

		width = r.x;
		height = r.y;
	}

	void createWindow(WindowData& data) override {
		SDL_Log("createWindow w: %i h: %i, refresh rate: %i", data.width, data.height, data.refreshRate);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		// GLFWmonitor* monitor = nullptr;
		// if(data.mode == WM_Fullscreen) {
		// 	monitor = glfwGetPrimaryMonitor();

		// 	if(!data.targetMonitor.empty()) {
		// 		int count = 0;
		// 		GLFWmonitor** monitors = glfwGetMonitors(&count);
		// 		for(int i = 0; i < count; ++i) {
		// 			if(data.targetMonitor == glfwGetMonitorName(monitors[i])) {
		// 				monitor = monitors[i];
		// 				break;
		// 			}
		// 		}
		// 	}

		// 	if(monitor && !data.overrideMonitor) {
		// 		int count = 0;
		// 		const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);
		// 		bool match = false;
		// 		for(int i = 0; i < count; ++i) {
		// 			auto& mode = modes[i];
		// 			if(mode.width == data.width && mode.height == data.height && (data.refreshRate == 0 || data.refreshRate == mode.refreshRate)) {
		// 				match = true;
		// 				break;
		// 			}
		// 		}

		// 		if(!match) {
		// 			monitor = nullptr;
		// 			data.mode = WM_Window;
		// 		}
		// 	}
		// }

		// if(data.mode != WM_Fullscreen && (data.width == 0 || data.height == 0)) {
		// 	GLFWmonitor* primary = glfwGetPrimaryMonitor();
		// 	const GLFWvidmode* mode = glfwGetVideoMode(primary);

		// 	data.width = mode->width;
		// 	data.height = mode->height;
		// }

		if (data.width == 0) {
			data.width = 800;
		}

		if (data.height == 0) {
			data.height = 600;
		}

		if (data.refreshRate == 0) {
			data.refreshRate = 60;
		}

		window = SDL_CreateWindow("Star Ruler 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data.width, data.height, SDL_WINDOW_OPENGL);

		if (!window) {
			error("Could not create window.");
			return;
		}

		glcontext = SDL_GL_CreateContext(window);
		if (!glcontext) {
			error("Couldn't create context.");
			return;
		}

		if (data.verticalSync) {
			if(SDL_GL_SetSwapInterval(1) < 0) {
				SDL_Log("Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
			}
		}

		SDL_GetWindowSize(window, &win_width, &win_height);

		if(data.mode == WM_Fullscreen)
			setCursorVisible(true);

		SDL_GL_SwapWindow(window);
		// glfwSetWindowCloseCallback(window, redirectWindowClose);
		// glfwSetWindowSizeCallback(window, redirectWindowResize);
		// glfwSetKeyCallback(window, redirectSpecialKey);
		// glfwSetCharCallback(window, redirectCharKey);
		// glfwSetMouseButtonCallback(window, redirectMouseButton);
		// glfwSetScrollCallback(window, redirectMouseWheel);
		// glfwSetCursorPosCallback(window, redirectMouseMove);
		// glfwSetCursorEnterCallback(window, trackMouseOver);

#ifdef WIN_MODE
		HICON hSmallIcon = (HICON) LoadImage ( 0, "sr2.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE | LR_DEFAULTCOLOR );
		SendMessage ( GetActiveWindow(), WM_SETICON, ICON_SMALL, (long)hSmallIcon );
#endif
	}

	void getVideoModes(std::vector<OSDriver::VideoMode>& output) override {
		SDL_Log("getVideoModes");
		int count = 0;
		output.reserve(count);
		output.resize(0);

		SDL_DisplayMode current;

		std::set<uint64_t> sizes;

		for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){
			SDL_Log("getVideoModes SDL_GetNumDisplayModes: %i", SDL_GetNumDisplayModes(i));

		}

		for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){
			int should_be_zero = SDL_GetDesktopDisplayMode(i, &current);

			if(should_be_zero != 0) {
				// In case of error...
				SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
			} else {
				// On success, print the current display mode.
				SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, current.w, current.h, current.refresh_rate);

				int numDisplayModes = SDL_GetNumDisplayModes(i);

				for( int j = 0; j < numDisplayModes; ++j )
				{
					SDL_DisplayMode mode;
					int display = 0;

					if( SDL_GetDisplayMode( should_be_zero, j, &mode ) < 0 )
						continue;

					if( !mode.w || !mode.h )
					{
						SDL_Log("Display supports any resolution");
					}

					SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i", j, SDL_BITSPERPIXEL(mode.format), SDL_GetPixelFormatName(mode.format), mode.w, mode.h);
					SDL_Log("Display mode w: %i h: %i, refresh_rate: %i", mode.w, mode.h, mode.refresh_rate);

					VideoMode m;
					m.width = mode.w;
					m.height = mode.h;
					m.refresh = mode.refresh_rate;

					uint64_t size = (uint64_t)m.width << 16 | (uint64_t)m.height | ((uint64_t)m.refresh << 32);

					if(sizes.find(size) == sizes.end()) {
						sizes.insert(size);
						output.push_back(m);
					}
				}
			}
		}
	}
	
	void getMonitorNames(std::vector<std::string>& output) override {
		int count = 0;
		int num_vide_displays = SDL_GetNumVideoDisplays();
		output.resize(num_vide_displays);
		for(int i = 0; i < count; ++i)
			output[i] = "Monitor?";
		
	}

	bool isWindowFocused() override {
		//return glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
		int flags = SDL_GetWindowFlags(window);
		return (flags & SDL_WINDOW_MOUSE_FOCUS);
	}

	bool isWindowMinimized() override {
		int flags = SDL_GetWindowFlags(window);
		//return glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0;
		return (flags & SDL_WINDOW_MINIMIZED);
	}

	void flashWindow() override {
		//glfwFlashWindow(window);
	}
	
	bool isMouseOver() override {
		return mouseOver;
	}

	void setClipboard(const std::string& text) override {
		SDL_SetClipboardText(text.c_str());
		//glfwSetClipboardString(window, text.c_str());
	}

	std::string getClipboard() override {
		 const char* str = SDL_GetClipboardText();
		 if(str)
			 return std::string(str);
		 return std::string();
	}

	int getCharForKey(int key) override {
		SDL_Log("getCharForKey:");
		//return SDL_GetScancodeFromKey(key);
		return 0;
	}

	int getKeyForChar(unsigned char chr) override {
		SDL_Log("getKeyForChar: %u", chr);
		//return SDL_GetScancodeFromKey(chr);
		return (int)(chr);
	}

	unsigned getDoubleClickTime() const override {
#ifdef _MSC_VER
		return GetDoubleClickTime();
#else
		//TODO: Get user's setting on linux
		return 200;
#endif
	}
	
	void getLastMousePos(int& x, int& y) override {
		x = mouse_x;
		y = mouse_y;
	}

	void getMousePos(int& x, int& y) override {
		//int x, y;
		SDL_GetMouseState(&x, &y);
	}

	void setMousePos(int x, int y) override {
		SDL_WarpMouseInWindow(window, x, y);
		//glfwSetCursorPos(window, x,y);
	}

	void setCursorVisible(bool visible) override {
		SDL_ShowCursor(visible);
		//glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
	}

	void setCursorLocked(bool locked) override {
		if(canLock == locked)
			return;
		canLock = locked;
		if(shouldLock) {
			// if(locked)
			// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
			// else
			// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_FREE);
		}
	}
	
	void setCursorShouldLock(bool locked) override {
		if(shouldLock == locked)
			return;
		shouldLock = locked;
		if(canLock) {
			// if(locked)
			// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
			// else
			// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_FREE);
		}
	}

	void sleep(int milliseconds) override {
		threads::sleep(milliseconds);
	}

	void resetTimer() override {
#ifndef WIN_MODE
		gettimeofday(&start_time, 0);
#else
		FILETIME cur_time;
		GetSystemTimeAsFileTime(&cur_time);
		start_time.HighPart = cur_time.dwHighDateTime;
		start_time.LowPart = cur_time.dwLowDateTime;

		QueryPerformanceFrequency(&start_time_hq_freq);
		QueryPerformanceCounter(&start_time_hq);
#endif
		frameTime = 0.0;
		resetGameTime(0);
	}

	int getTime() const override {
#ifndef WIN_MODE
		timeval cur_time;
		gettimeofday(&cur_time, 0);

		return (int)(
			1000 * (cur_time.tv_sec - start_time.tv_sec)
			+ (cur_time.tv_usec - start_time.tv_usec) / 1000);
#else

		FILETIME cur_ftime;
		GetSystemTimeAsFileTime(&cur_ftime);
		ULARGE_INTEGER cur_time;
		cur_time.HighPart = cur_ftime.dwHighDateTime;
		cur_time.LowPart = cur_ftime.dwLowDateTime;

		const double ticksPerSecond = 10000000.0;
		auto timeInSeconds = (double)(cur_time.QuadPart - start_time.QuadPart) / ticksPerSecond;

		return int(timeInSeconds * 1000.0);
#endif
	}

	double getAccurateTime() const override {
#ifndef WIN_MODE
		timeval cur_time;
		gettimeofday(&cur_time, 0);

		return (double)(cur_time.tv_sec - start_time.tv_sec)
			+ (double)(cur_time.tv_usec - start_time.tv_usec)/1000000.0;
#else
		LARGE_INTEGER cur_time, cur_freq;
		QueryPerformanceCounter(&cur_time);
		QueryPerformanceFrequency(&cur_freq);

		return (double)cur_time.QuadPart / (double)cur_freq.QuadPart - (double)start_time_hq.QuadPart / (double)start_time_hq_freq.QuadPart;
#endif
	}

	void resetGameTime(double time) override {
		//Game time starts slightly ahead of render time
		gameTime = time;
		gameSpeed = 1;
	}

	double getGameTime() const override {
		return gameTime;
	}
	
	double getFrameTime() const override {
		return frameTime;
	}

	double getGameSpeed() const override {
		return gameSpeed;
	}

	void setGameSpeed(double speed) override {
		gameSpeed = speed;
	}

	unsigned getProcessorCount() override {
		return std::max(threads::getNumberOfProcessors(),1u);
	}

	void setWindowTitle(const char* str) override {
		SDL_SetWindowTitle(window, str);
	}

	void setWindowSize(int width, int height) override {
		SDL_SetWindowSize(window, width, height);
	}

	void closeWindow() override {
		SDL_DestroyWindow(window);
	}
};

void redirectWindowClose(SDL_Window*) {
	driver->onWindowClose();
}

void redirectWindowResize(SDL_Window*, int w, int h) {
	driver->onResize(w,h);
	driver->win_width = w;
	driver->win_height = h;
	glViewport(0, 0, w, h);
}

void redirectSpecialKey(SDL_Window*, int key, int scan, int action, int mods) {
	SDL_Log("redirectSpecialKey: %i, %i, %i, %i", key, scan, action, mods);
	// driver->shiftKey = (mods & GLFW_MOD_SHIFT) != 0;
	// driver->ctrlKey = (mods & GLFW_MOD_CONTROL) != 0;
	// driver->altKey = (mods & GLFW_MOD_ALT) != 0;
	driver->shiftKey = false;
	driver->ctrlKey = false;
	driver->altKey = false;

	// bool pressed;
	int keyaction;
	keyaction = KA_Released;
	// switch(action) {
	// 	case GLFW_PRESS:
	// 		pressed = true;
	// 		keyaction = KA_Pressed;
	// 	break;
	// 	case GLFW_RELEASE:
	// 		pressed = false;
	// 		keyaction = KA_Released;
	// 	break;
	// 	case GLFW_REPEAT:
	// 		pressed = true;
	// 		keyaction = KA_Repeated;
	// 	break;
	// 	default:
	// 		return;
	// }

	return;

	// switch(key) {
	// 	case GLFW_KEY_LEFT_SHIFT:
	// 	case GLFW_KEY_RIGHT_SHIFT:
	// 		driver->shiftKey = pressed;
	// 	break;

	// 	case GLFW_KEY_LEFT_CONTROL:
	// 	case GLFW_KEY_RIGHT_CONTROL:
	// 		driver->ctrlKey = pressed;
	// 	break;

	// 	case GLFW_KEY_LEFT_ALT:
	// 	case GLFW_KEY_RIGHT_ALT:
	// 		driver->altKey = pressed;
	// 	break;
	// }

	driver->onKeyEvent(key, keyaction);
}

void redirectCharKey(SDL_Window*, unsigned key) {
	driver->onCharTyped(key);
}

void redirectMouseButton(SDL_Window*, int button, int action, int mods) {
	bool pressed = action == SDL_MOUSEBUTTONDOWN;
	int game_button = SR2_MOUSE_BUTTON_LEFT;
	
	// driver->shiftKey = (mods & GLFW_MOD_SHIFT) != 0;
	// driver->ctrlKey = (mods & GLFW_MOD_CONTROL) != 0;
	// driver->altKey = (mods & GLFW_MOD_ALT) != 0;

	switch(button) {
		case SDL_BUTTON_LEFT:
			game_button = SR2_MOUSE_BUTTON_LEFT;
			driver->leftButton = pressed;
		break;

		case SDL_BUTTON_RIGHT:
			game_button = SR2_MOUSE_BUTTON_RIGHT;
			driver->rightButton = pressed;
		break;

		case SDL_BUTTON_MIDDLE:
			game_button = SR2_MOUSE_BUTTON_MIDDLE;
			driver->middleButton = pressed;
		break;
	}

	driver->onMouseButton(game_button, pressed);
}

void redirectMouseMove(SDL_Window*, double x, double y) {
	driver->onMouseMoved((int)x, (int)y);
	driver->mouse_x = (int)x;
	driver->mouse_y = (int)y;
}

void redirectMouseWheel(SDL_Window*, double x, double y) {
	driver->onScroll(x, y);
}

void trackMouseOver(SDL_Window*, int over) {
	SDL_Log("trackMouseOver over: %i", over);
	driver->mouseOver = over == GL_TRUE;
}

void pollEvents(SDL_Window* window) {
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_MOUSEMOTION:
				redirectMouseMove(window, event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				redirectMouseButton(window, event.button.button, event.type, 0);
				break;
			case SDL_MOUSEBUTTONUP:
				redirectMouseButton(window, event.button.button, event.type, 0);
				break;
			case SDL_QUIT:
				redirectWindowClose(window);
				break;
		}
	}
}

OSDriver* getSDL2Driver() {
	if(!driver)
		driver = new SDL2Driver();
	return driver;
}

};
