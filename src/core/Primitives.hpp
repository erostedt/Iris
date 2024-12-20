#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Directions.hpp"
#include "Mesh.hpp"
#include "RenderObject.hpp"

namespace Iris
{

static inline RenderObject CreateCube()
{
    std::vector<glm::vec3> positions{
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},

        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},

        {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f, -0.5f},

        { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f, -0.5f},

        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},

        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
    };

    std::vector<glm::vec3> normals
    {
        FORWARD,
        FORWARD,
        FORWARD,
        FORWARD,

        BACKWARD,
        BACKWARD,
        BACKWARD,
        BACKWARD,

        LEFT,
        LEFT,
        LEFT,
        LEFT,

        RIGHT,
        RIGHT,
        RIGHT,
        RIGHT,

        DOWN,
        DOWN,
        DOWN,
        DOWN,

        UP,
        UP,
        UP,
        UP,
    };
    std::vector<glm::vec2> texture_coordinates
    {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    };

    std::vector<uint32_t> indices = {
    // Front face
    0, 1, 2, 0, 2, 3,
    // Back face
    4, 5, 6, 4, 6, 7,
    // Left face
    8, 9, 10, 8, 10, 11,
    // Right face
    12, 13, 14, 12, 14, 15,
    // Bottom face
    16, 17, 18, 16, 18, 19,
    // Top face
    20, 21, 22, 20, 22, 23
};

    auto mesh = std::make_unique<Mesh>(std::move(positions), std::move(normals), std::move(texture_coordinates), std::move(indices));
    return RenderObject(std::move(mesh));
}

static inline RenderObject CreateQuad()
{
    std::vector<glm::vec3> positions {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f}
    };

    std::vector<glm::vec3> normals {FORWARD, FORWARD, FORWARD, FORWARD};
    std::vector<glm::vec2> texture_coordinates {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}

    };
    std::vector<glm::vec4> colors;

    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    auto mesh = std::make_unique<Mesh>(std::move(positions), std::move(normals), std::move(texture_coordinates), std::move(indices));
    return RenderObject(std::move(mesh));
}

static inline RenderObject CreateTriangle()
{
    std::vector<glm::vec3> positions = {
        {-0.5f, -0.5f, 0.0f},
        {0.0f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f}
    };

    std::vector<glm::vec3> normals {FORWARD, FORWARD, FORWARD};
    std::vector<glm::vec2> texture_coordinates
    {
        {0.0f, 0.0f},
        {0.5f, 1.0f},
        {1.0f, 0.0f}
    };

    std::vector<uint32_t> indices = {0, 1, 2};
    auto mesh = std::make_unique<Mesh>(std::move(positions), std::move(normals), std::move(texture_coordinates), std::move(indices));
    return RenderObject(std::move(mesh));
}


static inline RenderObject CreateSphere(size_t latitudes = 20, size_t longitudes = 20, float radius = 0.5f)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texture_coordinates;

    std::vector<uint32_t> indices;
    positions.reserve((latitudes + 1) * (longitudes + 1));
    normals.reserve((latitudes + 1) * (longitudes + 1));
    texture_coordinates.reserve((latitudes + 1) * (longitudes + 1));
    indices.reserve(latitudes * longitudes * 6);

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
            positions.push_back({x, y, z});
            normals.push_back(glm::normalize(glm::vec3(x, y, z)));
            texture_coordinates.push_back({static_cast<float>(s) / longitudes, static_cast<float>(r) / latitudes});
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
    auto mesh = std::make_unique<Mesh>(std::move(positions), std::move(normals), std::move(texture_coordinates), std::move(indices));
    return RenderObject(std::move(mesh));
}


static inline RenderObject CreatePointCloud(const std::vector<glm::vec3>& centers, float radius, size_t latitudes = 20, size_t longitudes = 20)
{
    std::vector<float> sin_thetas;
    std::vector<float> cos_thetas;
    std::vector<float> sin_phis;
    std::vector<float> cos_phis;
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

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texture_coordinates;

    std::vector<uint32_t> indices;

    positions.reserve((latitudes + 1) * (longitudes + 1) * centers.size());
    normals.reserve((latitudes + 1) * (longitudes + 1) * centers.size());
    texture_coordinates.reserve((latitudes + 1) * (longitudes + 1) * centers.size());
    indices.reserve(latitudes * longitudes * 6 * centers.size());

    size_t offset = 0;
    for (auto center : centers)
    {
        for (size_t r = 0; r <= latitudes; ++r)
        {
            float r_sin_theta = radius * sin_thetas.at(r);
            float y = center.y + radius * cos_thetas.at(r);

            for (size_t s = 0; s <= longitudes; ++s)
            {
                float x = center.x + r_sin_theta * cos_phis.at(s);
                float z = center.z + r_sin_theta * sin_phis.at(s);

                positions.push_back({x, y, z});
                normals.push_back(glm::normalize(glm::vec3(x, y, z)));
                texture_coordinates.push_back({static_cast<float>(s) / longitudes, static_cast<float>(r) / latitudes});
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

    auto mesh = std::make_unique<Mesh>(std::move(positions), std::move(normals), std::move(texture_coordinates), std::move(indices));
    return RenderObject(std::move(mesh));
}

static inline RenderObject CreateVoxelMesh(const std::vector<glm::vec3>& centers, float width, float height, float depth)
{
    const size_t SINGLE_CUBE_VERTEX_POINTS = 24;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texture_coordinates;
    std::vector<uint32_t> indices;

    positions.reserve(SINGLE_CUBE_VERTEX_POINTS * centers.size());
    normals.reserve(SINGLE_CUBE_VERTEX_POINTS * centers.size());
    texture_coordinates.reserve(SINGLE_CUBE_VERTEX_POINTS * centers.size());
    indices.reserve(SINGLE_CUBE_VERTEX_POINTS * centers.size());


    std::vector<uint32_t> unshifted_indices = {
    // Front face
    0, 1, 2, 0, 2, 3,
    // Back face
    4, 5, 6, 4, 6, 7,
    // Left face
    8, 9, 10, 8, 10, 11,
    // Right face
    12, 13, 14, 12, 14, 15,
    // Bottom face
    16, 17, 18, 16, 18, 19,
    // Top face
    20, 21, 22, 20, 22, 23
};


    size_t offset = 0;
    float dx = 0.5f * width;
    float dy = 0.5f * height;
    float dz = 0.5f * depth;
    for (auto center : centers)
    {
        positions.push_back(center + glm::vec3(-dx, -dy, -dz));
        positions.push_back(center + glm::vec3(dx, -dy, -dz));
        positions.push_back(center + glm::vec3(dx, dy, -dz));
        positions.push_back(center + glm::vec3(-dx, dy, -dz));

        positions.push_back(center + glm::vec3(-dx, -dy, dz));
        positions.push_back(center + glm::vec3(dx, -dy, dz));
        positions.push_back(center + glm::vec3(dx, dy, dz));
        positions.push_back(center + glm::vec3(-dx, dy, dz));

        positions.push_back(center + glm::vec3(-dx, -dy, -dz));
        positions.push_back(center + glm::vec3(-dx, -dy, dz));
        positions.push_back(center + glm::vec3(-dx, dy, dz));
        positions.push_back(center + glm::vec3(-dx, dy, -dz));

        positions.push_back(center + glm::vec3(dx, -dy, -dz));
        positions.push_back(center + glm::vec3(dx, -dy, dz));
        positions.push_back(center + glm::vec3(dx, dy, dz));
        positions.push_back(center + glm::vec3(dx, dy, -dz));

        positions.push_back(center + glm::vec3(-dx, -dy, dz));
        positions.push_back(center + glm::vec3(dx, -dy, dz));
        positions.push_back(center + glm::vec3(dx, -dy, -dz));
        positions.push_back(center + glm::vec3(-dx, -dy, -dz));

        positions.push_back(center + glm::vec3(-dx, -dy, -dz));
        positions.push_back(center + glm::vec3(dx, dy, -dz));
        positions.push_back(center + glm::vec3(dx, dy, dz));
        positions.push_back(center + glm::vec3(-dx, dy, dz));

        //---------
        normals.push_back(FORWARD);
        normals.push_back(FORWARD);
        normals.push_back(FORWARD);
        normals.push_back(FORWARD);

        normals.push_back(BACKWARD);
        normals.push_back(BACKWARD);
        normals.push_back(BACKWARD);
        normals.push_back(BACKWARD);

        normals.push_back(LEFT);
        normals.push_back(LEFT);
        normals.push_back(LEFT);
        normals.push_back(LEFT);

        normals.push_back(RIGHT);
        normals.push_back(RIGHT);
        normals.push_back(RIGHT);
        normals.push_back(RIGHT);

        normals.push_back(DOWN);
        normals.push_back(DOWN);
        normals.push_back(DOWN);
        normals.push_back(DOWN);

        normals.push_back(UP);
        normals.push_back(UP);
        normals.push_back(UP);
        normals.push_back(UP);

        //--------
        texture_coordinates.push_back({0.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 1.0f});
        texture_coordinates.push_back({0.0f, 1.0f});
        
        texture_coordinates.push_back({0.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 1.0f});
        texture_coordinates.push_back({0.0f, 1.0f});

        texture_coordinates.push_back({0.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 1.0f});
        texture_coordinates.push_back({0.0f, 1.0f});

        texture_coordinates.push_back({0.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 1.0f});
        texture_coordinates.push_back({0.0f, 1.0f});

        texture_coordinates.push_back({0.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 1.0f});
        texture_coordinates.push_back({0.0f, 1.0f});

        texture_coordinates.push_back({0.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 0.0f});
        texture_coordinates.push_back({1.0f, 1.0f});
        texture_coordinates.push_back({0.0f, 1.0f});

        for (size_t index : unshifted_indices)
        {
            indices.push_back(index + offset);
        }
        offset += SINGLE_CUBE_VERTEX_POINTS;
    }
    auto mesh = std::make_unique<Mesh>(std::move(positions), std::move(normals), std::move(texture_coordinates), std::move(indices));
    return RenderObject(std::move(mesh));
}

} // namespace Iris
