#include "imgui.h"
#include <functional>
#include <unordered_map>
#include <vector>

class Controls
{
    using CallbackFunction = std::function<void()>;

  public:
    void AddOnDown(ImGuiKey key, const CallbackFunction on_down)
    {
        m_on_down[key].emplace_back(std::move(on_down));
    }
    void AddOnPressed(ImGuiKey key, const CallbackFunction on_pressed)
    {
        m_on_pressed[key].emplace_back(std::move(on_pressed));
    }
    void AddOnReleased(ImGuiKey key, const CallbackFunction on_released)
    {
        m_on_released[key].emplace_back(std::move(on_released));
    }

    void Invoke() const
    {
        for (const auto &[key, callback_functions] : m_on_down)
        {
            if (ImGui::IsKeyPressed(key, true))
            {
                InvokeCallbacks(callback_functions);
            }
        }
        for (const auto &[key, callback_functions] : m_on_pressed)
        {
            if (ImGui::IsKeyPressed(key, false))
            {
                InvokeCallbacks(callback_functions);
            }
        }
        for (const auto &[key, callback_functions] : m_on_released)
        {
            if (ImGui::IsKeyReleased(key))
            {
                InvokeCallbacks(callback_functions);
            }
        }
    }

  private:
    void InvokeCallbacks(const std::vector<CallbackFunction> &callbacks) const
    {
        for (const auto &callback : callbacks)
        {
            callback();
        }
    }

  private:
    std::unordered_map<ImGuiKey, std::vector<CallbackFunction>> m_on_down;
    std::unordered_map<ImGuiKey, std::vector<CallbackFunction>> m_on_pressed;
    std::unordered_map<ImGuiKey, std::vector<CallbackFunction>> m_on_released;
};
