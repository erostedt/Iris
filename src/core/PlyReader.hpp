#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

inline void swap_endianess(float &flt)
{
    char *byte = (char *)&flt;

    char tmp = byte[0];
    byte[0] = byte[3];
    byte[3] = tmp;

    tmp = byte[1];
    byte[1] = byte[2];
    byte[2] = tmp;
}

inline int first_int_in_string(std::string_view str_view)
{
    size_t curr = str_view.find_first_of("0123456789");
    std::stringstream ss;
    while (curr < str_view.length() && std::isdigit(str_view.at(curr)))
    {
        ss << str_view.at(curr);
        curr++;
    }

    int out;
    ss >> out;
    return out;
}

inline std::optional<std::vector<glm::vec3>> point_cloud_3d_from_ply(const std::filesystem::path &file_path)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open())
    {
        return {};
    }

    std::string line;
    int64_t num_vertices{0};
    while (std::getline(file, line))
    {

        if (line.find("vertex") != std::string::npos)
        {
            num_vertices = first_int_in_string(line);
        }

        if (line.find("end_header") != std::string::npos)
        {
            if (num_vertices == 0)
            {
                std::cout << "Could not find number of vertices" << std::endl;
                return {};
            }
            break;
        }
    }
    std::vector<glm::vec3> points;
    glm::vec3 point;
    for (int64_t i{0}; i < num_vertices; i++)
    {
        file.read(reinterpret_cast<char *>(&point), sizeof(point));
        swap_endianess(point.x);
        swap_endianess(point.y);
        swap_endianess(point.z);
        points.push_back(point);
    }

    return points;
}
