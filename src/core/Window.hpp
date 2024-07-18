#pragma once
#include "OpenGL.hpp"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static void GLFWErrorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

struct OpenGLVersion
{
    size_t major_version = 4;
    size_t minor_version = 6;
};

const OpenGLVersion OPEN_GL_330 = OpenGLVersion{3, 3};
const OpenGLVersion OPEN_GL_410 = OpenGLVersion{4, 1};
const OpenGLVersion OPEN_GL_460 = OpenGLVersion{4, 6};

class SimpleFrame
{
  public:
    SimpleFrame(GLFWwindow *window) : m_window{window}
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    ~SimpleFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (m_window)
        {
            glfwSwapBuffers(m_window);
        }
        glfwPollEvents();
    }
    GLFWwindow *m_window;
};

class SimpleWindow
{
  public:
    std::unique_ptr<SimpleFrame> NewFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return std::make_unique<SimpleFrame>(m_native_window);
    }

    bool Show() const
    {
        return !ShouldClose();
    }

    bool ShouldClose() const
    {
        return glfwWindowShouldClose(m_native_window);
    }

    GLFWwindow *GetNativeWindow() const
    {
        return m_native_window;
    }

    static std::unique_ptr<SimpleWindow> Create(const char *name, int width, int height,
                                                OpenGLVersion gl_version = OPEN_GL_410)
    {
        glfwSetErrorCallback(GLFWErrorCallback);
        if (!glfwInit())
            return {};

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version.major_version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version.minor_version);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow *window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (window == nullptr)
        {
            return {};
        }
        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
            return {};
        }

        glfwSwapInterval(1);
        glEnable(GL_DEPTH_TEST);

        std::cout << glGetString(GL_VERSION) << std::endl;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        auto simple_window = std::make_unique<SimpleWindow>();
        simple_window->m_native_window = window;

        return simple_window;
    }

    ~SimpleWindow()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        if (m_native_window)
        {
            glfwDestroyWindow(m_native_window);
        }
        glfwTerminate();
    }

  private:
    GLFWwindow *m_native_window;
};
