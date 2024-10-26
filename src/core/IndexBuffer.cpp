#include "OpenGL.hpp"

#include <cstdint>
#include <vector>

#include "IndexBuffer.hpp"

namespace Iris
{

    IndexBuffer::IndexBuffer(const std::vector<uint32_t> &&indices) : m_indices(indices)
    {
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
        Unbind();
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept
    {
        std::swap(m_indices, other.m_indices);
        m_id = other.m_id;
        other.m_id = 0;
    }
    IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept
    {
        std::swap(m_indices, other.m_indices);
        m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    size_t IndexBuffer::Size() const
    {
        return m_indices.size();
    }

}; // namespace Iris
