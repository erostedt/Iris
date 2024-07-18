#pragma once
#include "OpenGL.hpp"

#include <cstddef>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdint>
#include <cstdlib>
#include <stdio.h>

#include <alloca.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Exception.hpp"

static inline std::string shader_type(uint32_t type)
{
    switch (type)
    {
    case GL_FRAGMENT_SHADER:
        return "Fragment";

    case GL_VERTEX_SHADER:
        return "Vertex";

    default:
        return "Unknown";
    }
}

static inline uint32_t compile_shader(const uint32_t type, const std::string &code)
{
    uint32_t id = glCreateShader(type);
    const char *src = code.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = static_cast<char *>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << shader_type(type) << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
static inline std::string read_file(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cout << "Could not open file" << std::endl;
        return "";
    }
    std::stringstream ss;
    std::string line;
    while (std::getline(file, line))
    {
        ss << line << '\n';
    }

    return ss.str();
}

static inline uint32_t create_shader(const std::string &vertex_shader_path, const std::string &fragment_shader_path)
{
    uint32_t program = glCreateProgram();
    const std::string vsource = read_file(vertex_shader_path);
    const std::string fsource = read_file(fragment_shader_path);
    uint32_t vertex_shader = compile_shader(GL_VERTEX_SHADER, vsource);
    uint32_t fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fsource);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    // Find all uniform names and their types
    // Find all uniform locations
    // Make map from uniform name to Uniform
    return program;
}

/* This is example proto of how one could do this
template <typename T> class Uniform
{
  public:
    void Set(const T &new_value)
    {
        throw NotImplementedException(std::string("Set not implemented for type: ") + typeid(T).name());
    };
    uint32_t GetLocation()
    {
        return m_location;
    }
    const T &GetValue()
    {
        return m_value;
    }

  protected:
    uint32_t m_location;
    T m_value;
};

class Shader
{
};
*/
