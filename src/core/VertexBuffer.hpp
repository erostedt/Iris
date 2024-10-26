#pragma once

#include "OpenGL.hpp"

#include <vector>

#include "Buffer.hpp"
#include "Vertex.hpp"
namespace Iris
{

template <typename T> class VertexAttributeBuffer : Buffer
{
  public:
    VertexAttributeBuffer(const std::vector<T> &&vertices, const VertexLayout &layout, uint32_t usage=GL_STATIC_DRAW)
        : m_data(vertices), m_layout(layout)
    {
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), usage);
        Unbind();
    }

    virtual ~VertexAttributeBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    VertexAttributeBuffer(const VertexAttributeBuffer &other) = delete;
    VertexAttributeBuffer &operator=(const VertexAttributeBuffer &other) = delete;

    VertexAttributeBuffer(VertexAttributeBuffer &&other) noexcept
    {
        std::swap(m_data, other.m_data);
        m_layout = other.m_layout;
        m_id = other.m_id;
        other.m_id = 0;
    }
    VertexAttributeBuffer &operator=(VertexAttributeBuffer &&other) noexcept
    {
        std::swap(m_data, other.m_data);
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

    const std::vector<T> &GetVertices() const
    {
        return m_data;
    }

    const size_t Size() const
    {
        return m_data.size();
    }

  private:
    std::vector<T> m_data;
    VertexLayout m_layout;
    uint32_t m_id{0};
};

}; // namespace Iris
