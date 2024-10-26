#pragma once
#include "RenderObject.hpp"
#include "Vertex.hpp"

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Hash.hpp"
#include "Mesh.hpp"

namespace Iris
{

using VertexIndices = std::vector<uint32_t>;

static VertexIndices parse_vertex_indices(const std::string &vertex_indices_string)
{
    std::istringstream stream(vertex_indices_string);
    std::string index_string;
    VertexIndices vertex_indices;
    while (std::getline(stream, index_string, '/'))
    {
        uint32_t index;
        std::istringstream(index_string) >> index;
        vertex_indices.push_back(index - 1);
    }
    return vertex_indices;
}

static std::vector<VertexIndices> parse_face_indices(const std::string &line)
{
    std::string vertex_indices_string;
    std::istringstream stream(line);
    stream >> vertex_indices_string;

    std::vector<VertexIndices> face_indices;
    while (stream >> vertex_indices_string)
    {
        VertexIndices vertex_indices = parse_vertex_indices(vertex_indices_string);
        face_indices.push_back(std::move(vertex_indices));
    }
    return face_indices;
}

static inline RenderObject<TextureVertex> read_obj_file(const std::filesystem::path &objpath)
{
    assert(std::filesystem::exists(objpath));
    std::ifstream file(objpath);

    const std::string vertex_start("v ");
    const std::string texture_coordinate_start("vt ");
    const std::string normal_start("vn ");
    const std::string face_start("f ");
    std::string temp;

    std::string line;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texture_coordinates;
    std::vector<glm::vec3> normals;

    std::vector<TextureVertex> vertices;
    std::vector<uint32_t> indices;

    std::unordered_map<std::vector<uint32_t>, uint32_t, VectorHasher> seen_vertices;
    uint32_t current_index = 0;
    while (std::getline(file, line))
    {
        if (line.starts_with(vertex_start))
        {
            std::istringstream stream(line);
            stream >> temp;

            glm::vec3 position;
            stream >> position.x;
            stream >> position.y;
            stream >> position.z;
            positions.push_back(position);
        }
        else if (line.starts_with(texture_coordinate_start))
        {
            std::istringstream stream(line);
            stream >> temp;

            glm::vec2 texture_coordinate;
            stream >> texture_coordinate.x;
            stream >> texture_coordinate.y;
            texture_coordinates.push_back(texture_coordinate);
        }
        else if (line.starts_with(normal_start))
        {
            std::istringstream stream(line);
            stream >> temp;

            glm::vec3 normal;
            stream >> normal.x;
            stream >> normal.y;
            stream >> normal.z;
            normals.push_back(normal);
        }
        else if (line.starts_with(face_start))
        {
            const auto face_indices = parse_face_indices(line);

            for (const auto &vertex_indices : face_indices)
            {
                if (auto pos = seen_vertices.find(vertex_indices); pos != seen_vertices.end())
                {
                    indices.push_back(pos->second);
                }
                else
                {
                    TextureVertex vertex;
                    vertex.position = positions.at(vertex_indices[0]);
                    vertex.texture_coordinates = texture_coordinates.at(vertex_indices[1]);
                    vertex.normal = normals.at(vertex_indices[2]);

                    seen_vertices[vertex_indices] = current_index;
                    indices.push_back(current_index);
                    vertices.push_back(vertex);
                    current_index++;
                }
            }
        }
    }

    auto mesh = std::make_unique<Mesh>(std::move(vertices), std::move(indices));
    return RenderObject<TextureVertex>(std::move(mesh));
}

}; // namespace Iris
