#pragma once

#if defined(__APPLE__) && defined(__MACH__)
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
// Without this gl.h gets included instead of gl3.h
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#elif defined(__linux__)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else

#endif

struct OpenGLVersion
{
    size_t major_version = 4;
    size_t minor_version = 6;
};

static const OpenGLVersion OPEN_GL_330 = OpenGLVersion{3, 3};
static const OpenGLVersion OPEN_GL_410 = OpenGLVersion{4, 1};
static const OpenGLVersion OPEN_GL_460 = OpenGLVersion{4, 6};
