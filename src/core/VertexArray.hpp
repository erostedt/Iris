#pragma once

#include "OpenGL.hpp"

#include "VertexBuffer.hpp"

namespace Iris
{

template <typename T> class VertexArray : Buffer
{
  public:
    VertexArray(const FixedVertexBuffer<T> &vertex_buffer)
    {
        glGenVertexArrays(1, &m_id);
        AddVertexLayout(vertex_buffer);
        Unbind();
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

  private:
    void AddVertexLayout(const FixedVertexBuffer<T> &vertex_buffer)
    {
        Bind();
        vertex_buffer.Bind();
        const VertexLayout &layout = vertex_buffer.Layout();
        const auto &elements = layout.Elements();
        uint64_t offset = 0;
        for (unsigned int i = 0; i < elements.size(); ++i)
        {
            const auto &element = elements[i];

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.gl_type, element.normalized, layout.Stride(),
                                  (const void *)offset);
            offset += element.count * element.type_bytes;
        }
    }

  private:
    uint32_t m_id;
};

}; // namespace Iris
