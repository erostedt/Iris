#include <glm/glm.hpp>

#include "Color.hpp"
#include "Random.hpp"

namespace Iris
{

Color RandomRGB()
{
    return {Random::Uniform(0.0f, 1.0f), Random::Uniform(0.0f, 1.0f), Random::Uniform(0.0f, 1.0f), 1.0f};
}

ByteColor FloatColorToByte(Color color)
{
    uint8_t red = static_cast<uint8_t>(255.0f * color.r);
    uint8_t green = static_cast<uint8_t>(255.0f * color.g);
    uint8_t blue = static_cast<uint8_t>(255.0f * color.b);
    uint8_t alpha = static_cast<uint8_t>(255.0f * color.a);
    return {red, green, blue, alpha};
}


};
