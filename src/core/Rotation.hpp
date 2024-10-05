#pragma once

#include <glm/gtc/type_ptr.hpp>

static inline float UnwindRadians(float radians)
{
    const float TWO_PI = 2.0f * glm::pi<float>();
    radians = std::fmod(radians, TWO_PI);
    if (radians < 0.0f)
    {
        radians += TWO_PI;
    }
    return radians;
}
