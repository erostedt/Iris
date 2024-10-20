#pragma once
#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Color.hpp"
#include "Directions.hpp"
#include "Mesh.hpp"
#include "RenderObject.hpp"
#include "Shapes.hpp"
#include "Vertex.hpp"

namespace Iris
{

static inline RenderObject<Vertex> CreateTriangle(const Color &color)
{
    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 1.0f}, color},
        {{0.0f, 0.5f, 1.0f}, color},
        {{0.5f, -0.5f, 1.0f}, color},
    };

    std::vector<uint32_t> indices = {0, 1, 2};
    auto mesh = std::make_unique<Mesh<Vertex>>(std::move(vertices), std::move(indices));
    return RenderObject<Vertex>(std::move(mesh));
}

static inline RenderObject<TextureVertex> CreateQuad()
{
    std::vector<TextureVertex> vertices = {{{-0.5f, -0.5f, 0.0f}, FORWARD, {0.0f, 0.0f}},
                                           {{0.5f, -0.5f, 0.0f}, FORWARD, {1.0f, 0.0f}},
                                           {{0.5f, 0.5f, 0.0f}, FORWARD, {1.0f, 1.0f}},
                                           {{-0.5f, 0.5f, 0.0f}, FORWARD, {0.0f, 1.0f}}};

    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    auto mesh = std::make_unique<Mesh<TextureVertex>>(std::move(vertices), std::move(indices));
    return RenderObject<TextureVertex>(std::move(mesh));
}

static inline RenderObject<Vertex> CreateCube(const Color &color)
{
    std::vector<Vertex> vertices = {{{-0.5f, -0.5f, -0.5f}, color}, {{0.5f, -0.5f, -0.5f}, color},
                                    {{0.5f, 0.5f, -0.5f}, color},   {{-0.5f, 0.5f, -0.5f}, color},
                                    {{-0.5f, -0.5f, 0.5f}, color},  {{0.5f, -0.5f, 0.5f}, color},
                                    {{0.5f, 0.5f, 0.5f}, color},    {{-0.5f, 0.5f, 0.5f}, color}};

    std::vector<uint32_t> indices = {0, 1, 2,

                                     2, 3, 0,

                                     4, 5, 6,

                                     6, 7, 4,

                                     0, 4, 7,

                                     7, 3, 0,

                                     1, 5, 6,

                                     6, 2, 1,

                                     0, 1, 5,

                                     5, 4, 0,

                                     3, 2, 6,

                                     6, 7, 3};

    auto mesh = std::make_unique<Mesh<Vertex>>(std::move(vertices), std::move(indices));
    return RenderObject<Vertex>(std::move(mesh));
}

static inline RenderObject<Vertex> CreateSphere(const Color &color, size_t latitudes = 20, size_t longitudes = 20,
                                                float radius = 0.5f)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (size_t r = 0; r <= latitudes; ++r)
    {
        float theta = r * glm::pi<float>() / latitudes;
        float r_sin_theta = radius * std::sin(theta);
        float y = radius * std::cos(theta);

        for (size_t s = 0; s <= longitudes; ++s)
        {
            float phi = s * 2.0f * glm::pi<float>() / longitudes;
            float x = r_sin_theta * std::cos(phi);
            float z = r_sin_theta * std::sin(phi);

            vertices.push_back({{x, y, z}, color});
        }
    }

    for (size_t r = 0; r < latitudes; ++r)
    {
        const uint32_t current_offset = r * (longitudes + 1);
        const uint32_t next_offset = (r + 1) * (longitudes + 1);

        for (size_t s = 0; s < longitudes; ++s)
        {
            indices.push_back(current_offset + s);
            indices.push_back(next_offset + (s + 1));
            indices.push_back(next_offset + s);

            indices.push_back(current_offset + s);
            indices.push_back(current_offset + (s + 1));
            indices.push_back(next_offset + (s + 1));
        }
    }
    auto mesh = std::make_unique<Mesh<Vertex>>(std::move(vertices), std::move(indices));
    return RenderObject<Vertex>(std::move(mesh));
}

static inline RenderObject<Vertex> CreatePointCloud(const std::vector<ColoredSphere> &colored_spheres,
                                                    size_t latitudes = 20, size_t longitudes = 20)
{
    std::vector<float> sin_thetas{};
    std::vector<float> cos_thetas{};
    std::vector<float> sin_phis{};
    std::vector<float> cos_phis{};
    sin_thetas.reserve(latitudes + 1);
    cos_thetas.reserve(latitudes + 1);
    sin_phis.reserve(longitudes + 1);
    cos_phis.reserve(longitudes + 1);

    for (size_t r = 0; r <= latitudes; ++r)
    {
        float theta = r * glm::pi<float>() / latitudes;
        sin_thetas.push_back(std::sin(theta));
        cos_thetas.push_back(std::cos(theta));
    }
    for (size_t s = 0; s <= longitudes; ++s)
    {
        float phi = s * 2.0f * glm::pi<float>() / longitudes;
        sin_phis.push_back(std::sin(phi));
        cos_phis.push_back(std::cos(phi));
    }

    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};

    size_t offset = 0;
    for (const auto &point : colored_spheres)
    {
        const auto &center = point.center;

        for (size_t r = 0; r <= latitudes; ++r)
        {
            float r_sin_theta = point.radius * sin_thetas.at(r);
            float y = center.y + point.radius * cos_thetas.at(r);

            for (size_t s = 0; s <= longitudes; ++s)
            {
                float x = center.x + r_sin_theta * cos_phis.at(s);
                float z = center.z + r_sin_theta * sin_phis.at(s);

                vertices.push_back({{x, y, z}, point.color});
            }
        }

        for (size_t r = 0; r < latitudes; ++r)
        {
            const uint32_t current_offset = r * (longitudes + 1) + offset;
            const uint32_t next_offset = (r + 1) * (longitudes + 1) + offset;

            for (size_t s = 0; s < longitudes; ++s)
            {
                indices.push_back(current_offset + s);
                indices.push_back(next_offset + (s + 1));
                indices.push_back(next_offset + s);

                indices.push_back(current_offset + s);
                indices.push_back(current_offset + (s + 1));
                indices.push_back(next_offset + (s + 1));
            }
        }
        offset += (latitudes + 1) * (longitudes + 1);
    }

    auto mesh = std::make_unique<Mesh<Vertex>>(std::move(vertices), std::move(indices));
    return RenderObject<Vertex>(std::move(mesh));
}

static inline RenderObject<Vertex> CreateVoxelMesh(const std::vector<ColoredVoxel> &colored_voxels)
{
    const size_t VOXEL_CORNERS = 8;
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
    vertices.reserve(VOXEL_CORNERS * colored_voxels.size());
    std::array<uint32_t, 36> unshifted_indicies = {0, 1, 2,

                                                   2, 3, 0,

                                                   4, 5, 6,

                                                   6, 7, 4,

                                                   0, 4, 7,

                                                   7, 3, 0,

                                                   1, 5, 6,

                                                   6, 2, 1,

                                                   0, 1, 5,

                                                   5, 4, 0,

                                                   3, 2, 6,

                                                   6, 7, 3};

    size_t offset = 0;
    for (const auto &voxel : colored_voxels)
    {
        float dx = 0.5f * voxel.dimensions.x;
        float dy = 0.5f * voxel.dimensions.y;
        float dz = 0.5f * voxel.dimensions.z;

        vertices.push_back({voxel.center + glm::vec3(-dx, -dy, -dz), voxel.color});
        vertices.push_back({voxel.center + glm::vec3(dx, -dy, -dz), voxel.color});
        vertices.push_back({voxel.center + glm::vec3(dx, dy, -dz), voxel.color});
        vertices.push_back({voxel.center + glm::vec3(-dx, dy, -dz), voxel.color});
        vertices.push_back({voxel.center + glm::vec3(-dx, -dy, dz), voxel.color});
        vertices.push_back({voxel.center + glm::vec3(dx, -dy, dz), voxel.color});
        vertices.push_back({voxel.center + glm::vec3(dx, dy, dz), voxel.color});
        vertices.push_back({voxel.center + glm::vec3(-dx, dy, dz), voxel.color});

        for (size_t index : unshifted_indicies)
        {
            indices.push_back(index + offset);
        }
        offset += VOXEL_CORNERS;
    }
    auto mesh = std::make_unique<Mesh<Vertex>>(std::move(vertices), std::move(indices));
    return RenderObject<Vertex>(std::move(mesh));
}

} // namespace Iris
