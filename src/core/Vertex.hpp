#pragma once
#include "OpenGL.hpp"

#include <glm/glm.hpp>
#include <initializer_list>
#include <vector>

#include "Color.hpp"

struct VertexElement
{
    uint32_t gl_type;
    uint32_t type_bytes;
    uint32_t count;
    uint8_t normalized;
};

class VertexLayout
{
  public:
    VertexLayout(std::initializer_list<VertexElement> vertex_elements)
    {
        for (const auto &element : vertex_elements)
        {
            PushVertexElement(element);
        }
    }

    void PushVertexElement(const VertexElement &element)
    {
        m_elements.push_back(element);
        m_stride += element.count * element.type_bytes;
    }

    const std::vector<VertexElement> &Elements() const
    {
        return m_elements;
    };

    const size_t Stride() const
    {
        return m_stride;
    };

  private:
    std::vector<VertexElement> m_elements{};
    size_t m_stride{0};
};

struct Vertex
{
    glm::vec3 position;
    Color color;

    static VertexLayout Layout()
    {
        return {
            {GL_FLOAT, sizeof(float), static_cast<uint32_t>(glm::vec3::length()), GL_FALSE},
            {GL_FLOAT, sizeof(float), static_cast<uint32_t>(glm::vec4::length()), GL_FALSE},
        };
    }
};
