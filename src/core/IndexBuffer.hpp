#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>

#include "Buffer.hpp"

namespace Iris
{

class FixedIndexBuffer : Buffer
{
  public:
    FixedIndexBuffer(const std::vector<uint32_t> &&indices) : m_indices(indices)
    {
        glGenBuffers(1, &m_id);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
        Unbind();
    }

    virtual ~FixedIndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    FixedIndexBuffer(const FixedIndexBuffer &other) = delete;
    FixedIndexBuffer &operator=(const FixedIndexBuffer &other) = delete;

    FixedIndexBuffer(FixedIndexBuffer &&other) noexcept
    {
        std::swap(m_indices, other.m_indices);
        m_id = other.m_id;
        other.m_id = 0;
    }
    FixedIndexBuffer &operator=(FixedIndexBuffer &&other) noexcept
    {
        std::swap(m_indices, other.m_indices);
        m_id = other.m_id;
        other.m_id = 0;
        return *this;
    }

    void Bind() const override
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void Unbind() const override
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    size_t Size() const
    {
        return m_indices.size();
    }

    const std::vector<uint32_t> &GetIndices() const
    {
        return m_indices;
    }

  private:
    std::vector<uint32_t> m_indices;
    uint32_t m_id;
};

}; // namespace Iris
