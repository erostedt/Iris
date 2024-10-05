#include "OpenGL.hpp"

#include <cstdint>
#include <vector>

#include "IndexBuffer.hpp"

namespace Iris
{

    FixedIndexBuffer::FixedIndexBuffer(const std::vector<uint32_t> &&indices) : m_indices(indices)
    {
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
        Unbind();
    }

    FixedIndexBuffer::~FixedIndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    FixedIndexBuffer::FixedIndexBuffer(FixedIndexBuffer &&other) noexcept
    {
        std::swap(m_indices, other.m_indices);
        m_id = other.m_id;
        other.m_id = 0;
    }
    FixedIndexBuffer &FixedIndexBuffer::operator=(FixedIndexBuffer &&other) noexcept
    {
        std::swap(m_indices, other.m_indices);
        m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    void FixedIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void FixedIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    size_t FixedIndexBuffer::Size() const
    {
        return m_indices.size();
    }

    const std::vector<uint32_t> &FixedIndexBuffer::GetIndices() const
    {
        return m_indices;
    }

}; // namespace Iris
