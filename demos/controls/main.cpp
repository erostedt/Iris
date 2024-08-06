#include <iostream>

#include "Controls.hpp"
#include "Window.hpp"

using namespace Iris;

int main()
{
    const auto w = Window::Create("Window", 400, 300);
    Controls controls;
    controls.AddOnDown(ImGuiKey_W, []() { std::cout << "Holding W" << std::endl; });
    controls.AddOnReleased(ImGuiKey_W, []() { std::cout << "Released W" << std::endl; });
    controls.AddOnPressed(ImGuiKey_S, []() { std::cout << "Pressed S" << std::endl; });
    controls.AddOnPressed(ImGuiKey_Q, [&w]() { w->Close(); });

    while (w->Show())
    {
        const auto frame = w->NewFrame();
        controls.Invoke();
    }
}
