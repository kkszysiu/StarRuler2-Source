#pragma once
#ifndef ANDROID
#include <GL/glew.h>
#else
#include <GL/gl.h>
#endif
#ifdef USE_GLFW
#include "GLFW/glfw3.h"
#elif USE_SDL2
#include "SDL.h"
#endif
