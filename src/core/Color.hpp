#pragma once

#include <glm/glm.hpp>

using Color = glm::vec4;
using ByteColor = glm::vec<4, uint8_t>;

namespace Iris
{

static inline Color WHITE{1.0f, 1.0f, 1.0f, 1.0f};
static inline Color BLACK{0.0f, 0.0f, 0.0f, 1.0f};
static inline Color RED{1.0f, 0.0f, 0.0f, 1.0f};
static inline Color GREEN{0.0f, 1.0f, 0.0f, 1.0f};
static inline Color BLUE{0.0f, 0.0f, 1.0f, 1.0f};

Color RandomRGB();
ByteColor FloatColorToByte(Color color);

}; // namespace Iris
