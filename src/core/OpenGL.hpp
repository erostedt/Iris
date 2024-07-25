#pragma once

#include <iostream>
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
    size_t major_version;
    size_t minor_version;
};

static const OpenGLVersion OPEN_GL_330 = OpenGLVersion{3, 3};
static const OpenGLVersion OPEN_GL_410 = OpenGLVersion{4, 1};
static const OpenGLVersion OPEN_GL_460 = OpenGLVersion{4, 6};

static inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

static inline void LogGLError(const char *stmt, const char *fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "[OpenGL error] " << std::hex << err << " at " << fname << ":" << line << " - for " << stmt
                  << std::endl;
    }
}

static inline void LogAllGLErrors()
{
    while (true)
    {
        auto err = glGetError();
        if (err == GL_NO_ERROR)
        {
            return;
        }
        std::cerr << "[OpenGL error] " << std::hex << err << std::endl;
    }
}

#define GL_CHECK(x)                                                                                                    \
    GLClearError();                                                                                                    \
    x;                                                                                                                 \
    LogGLError(#x, __FILE__, __LINE__);
