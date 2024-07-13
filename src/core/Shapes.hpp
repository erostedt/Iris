#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include "Color.hpp"

namespace Iris
{

struct Voxel
{
    glm::vec3 center;
    glm::vec3 dimensions;
};

struct ColoredVoxel : Voxel
{
    Color color;
};

struct Sphere
{
    glm::vec3 center;
    float radius;
};

struct ColoredSphere : Sphere
{
    Color color;
};

}; // namespace Iris
