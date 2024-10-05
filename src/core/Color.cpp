#include <glm/glm.hpp>

#include "Color.hpp"
#include "Random.hpp"

namespace Iris
{

Color RandomRGB()
{
    return {Random::Uniform(0.0f, 1.0f), Random::Uniform(0.0f, 1.0f), Random::Uniform(0.0f, 1.0f), 1.0f};
}

};
