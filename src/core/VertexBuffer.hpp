#pragma once
#include "OpenGL.hpp"

#include <vector>

#include "Buffer.hpp"
#include "Vertex.hpp"
namespace Iris
{

template <typename T> class FixedVertexBuffer : Buffer
{
  public:
    FixedVertexBuffer(const std::vector<Vertex> &&vertices, const VertexLayout &layout)
        : m_vertices(vertices), m_layout(layout)
    {
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        Unbind();
    }

    virtual ~FixedVertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    FixedVertexBuffer(const FixedVertexBuffer &other) = delete;
    FixedVertexBuffer &operator=(const FixedVertexBuffer &other) = delete;

    FixedVertexBuffer(FixedVertexBuffer &&other) noexcept
    {
        std::swap(m_vertices, other.m_vertices);
        m_layout = other.m_layout;
        m_id = other.m_id;
        other.m_id = 0;
    }
    FixedVertexBuffer &operator=(FixedVertexBuffer &&other) noexcept
    {
        std::swap(m_vertices, other.m_vertices);
        m_layout = other.m_layout;
        m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    const VertexLayout &Layout() const
    {
        return m_layout;
    }

    void Bind() const override
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void Unbind() const override
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const std::vector<Vertex> &GetVertices() const
    {
        return m_vertices;
    }

    const size_t Size() const
    {
        return m_vertices.size();
    }

  private:
    std::vector<Vertex> m_vertices;
    VertexLayout m_layout;
    uint32_t m_id{0};
};

}; // namespace Iris
