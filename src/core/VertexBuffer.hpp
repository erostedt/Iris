#pragma once

#include "OpenGL.hpp"

#include <vector>

#include "Buffer.hpp"
namespace Iris
{

template <typename T> class VertexAttributeBuffer : Buffer
{
  public:
    VertexAttributeBuffer(const std::vector<T> &&vertices, uint32_t usage=GL_STATIC_DRAW)
        : m_data(vertices)
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
        m_id = other.m_id;
        other.m_id = 0;
    }
    VertexAttributeBuffer &operator=(VertexAttributeBuffer &&other) noexcept
    {
        std::swap(m_data, other.m_data);
        m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    void Bind() const override
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void Unbind() const override
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const size_t Size() const
    {
        return m_data.size();
    }

  private:
    std::vector<T> m_data;
    uint32_t m_id{0};
};

}; // namespace Iris
