#pragma once

#include "imgui.h"
#include <functional>
#include <unordered_map>
#include <vector>

namespace Iris
{

class Controls
{
    using CallbackFunction = std::function<void()>;

  public:
    void AddOnDown(ImGuiKey key, const CallbackFunction on_down);
    void AddOnPressed(ImGuiKey key, const CallbackFunction on_pressed);
    void AddOnReleased(ImGuiKey key, const CallbackFunction on_released);
    void Invoke() const;

  private:
    void InvokeCallbacks(const std::vector<CallbackFunction> &callbacks) const;

  private:
    std::unordered_map<ImGuiKey, std::vector<CallbackFunction>> m_on_down;
    std::unordered_map<ImGuiKey, std::vector<CallbackFunction>> m_on_pressed;
    std::unordered_map<ImGuiKey, std::vector<CallbackFunction>> m_on_released;
};
}; // namespace Iris
