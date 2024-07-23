#pragma once
#include "Color.hpp"
#include "OpenGL.hpp"

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

namespace Iris
{

class Frame
{
  public:
    Frame(GLFWwindow *window) : m_window{window}
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    ~Frame()
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

class Window
{
  public:
    std::unique_ptr<Frame> NewFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return std::make_unique<Frame>(m_native_window);
    }

    void SetBackgroundColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void SetBlendingFunction(const uint32_t source_factor, const uint32_t destination_factor)
    {
        glEnable(GL_BLEND);
        glBlendFunc(source_factor, destination_factor);
    }

    bool Show() const
    {
        return !ShouldClose();
    }

    bool ShouldClose() const
    {
        return glfwWindowShouldClose(m_native_window);
    }

    void Close() const
    {
        glfwSetWindowShouldClose(m_native_window, GL_TRUE);
    }

    GLFWwindow *GetNativeWindow() const
    {
        return m_native_window;
    }

    static std::unique_ptr<Window> Create(const char *name, int width, int height,
                                          OpenGLVersion gl_version = OPEN_GL_410)
    {
        glfwSetErrorCallback(GLFWErrorCallback);
        if (!glfwInit())
            return {};

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version.major_version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version.minor_version);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow *glfw_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (glfw_window == nullptr)
        {
            return {};
        }
        glfwMakeContextCurrent(glfw_window);

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

        ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
        ImGui_ImplOpenGL3_Init();
        auto window = std::make_unique<Window>();
        window->m_native_window = glfw_window;
        window->SetBackgroundColor(BLACK);
        window->SetBlendingFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return window;
    }

    ~Window()
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

}; // namespace Iris
