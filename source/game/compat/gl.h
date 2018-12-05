#pragma once
#ifdef __ANDROID__
#include <GL/glew.h>
#endif
#ifdef USE_GLFW
#include "GLFW/glfw3.h"
#elif USE_SDL2
#include "SDL.h"
#endif
