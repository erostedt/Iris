#pragma once

#if defined(__APPLE__) && defined(__MACH__)
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
// Without this gl.h gets included instead of gl3.h
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// For includes related to OpenGL, make sure their are included after glfw3.h
//#include <OpenGL/gl3.h>
#elif defined(__linux__)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else

#endif
