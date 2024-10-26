#pragma once

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

namespace Iris
{

template <typename T> class Mesh
{
  public:
    Mesh(std::vector<T> &&vertices, std::vector<uint32_t> &&indices)
        : m_vertex_buffer(std::move(vertices)), m_vertex_array(m_vertex_buffer),
          m_index_buffer(std::move(indices))
    {
    }

    void Bind() const
    {
        m_vertex_array.Bind();
        m_vertex_buffer.Bind();
        m_index_buffer.Bind();
    }

    void Unbind() const
    {
        m_vertex_array.Unbind();
        m_vertex_buffer.Unbind();
        m_index_buffer.Unbind();
    }

    const std::vector<T> &GetVertices() const
    {
        return m_vertex_buffer.GetVertices();
    }

    size_t NumVertices() const
    {
        return m_vertex_buffer.Size();
    }

    const std::vector<uint32_t> &GetIndices() const
    {
        return m_index_buffer.GetIndices();
    }

    size_t NumIndices() const
    {
        return m_index_buffer.Size();
    }

  private:
    VertexAttributeBuffer<T> m_vertex_buffer;
    VertexArray<T> m_vertex_array;
    FixedIndexBuffer m_index_buffer;
};

} // namespace Iris
