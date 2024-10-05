#include "imgui.h"
#include <functional>
#include <unordered_map>
#include <vector>

#include "Controls.hpp"

namespace Iris
{
    void Controls::AddOnDown(ImGuiKey key, const CallbackFunction on_down)
    {
        m_on_down[key].emplace_back(std::move(on_down));
    }

    void Controls::AddOnPressed(ImGuiKey key, const CallbackFunction on_pressed)
    {
        m_on_pressed[key].emplace_back(std::move(on_pressed));
    }
    void Controls::AddOnReleased(ImGuiKey key, const CallbackFunction on_released)
    {
        m_on_released[key].emplace_back(std::move(on_released));
    }

    void Controls::Invoke() const
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

    void Controls::InvokeCallbacks(const std::vector<CallbackFunction> &callbacks) const
    {
        for (const auto &callback : callbacks)
        {
            callback();
        }
    }
};
