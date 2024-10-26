#pragma once

#include "OpenGL.hpp"

#include "Vertex.hpp"
#include "VertexBuffer.hpp"

namespace Iris
{

class VertexArray : Buffer
{
  public:
    VertexArray()
    {
        glGenVertexArrays(1, &m_id);
    }

    void Bind() const override
    {
        glBindVertexArray(m_id);
    }

    void Unbind() const override
    {
        glBindVertexArray(0);
    }

    virtual ~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    VertexArray(const VertexArray &other) = delete;
    VertexArray &operator=(const VertexArray &other) = delete;

    VertexArray(VertexArray &&other) noexcept
    {
        m_id = other.m_id;
        other.m_id = 0;
    }
    VertexArray &operator=(VertexArray &&other) noexcept
    {
        m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    template <typename VectorType>
    void AddVertexAttribute(const VertexAttributeBuffer<VectorType> &vertex_buffer)
    {
        Bind();
        vertex_buffer.Bind();
        glEnableVertexAttribArray(m_vertex_attribute_count);
        glVertexAttribPointer(m_vertex_attribute_count, static_cast<uint32_t>(VectorType::length()), GL_FLOAT, GL_FALSE, 0, (const void *)0);
        ++m_vertex_attribute_count;
    }

  private:
    uint32_t m_vertex_attribute_count{0};
    uint32_t m_id;
};

}; // namespace Iris
