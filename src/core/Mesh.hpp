#pragma once

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

namespace Iris
{

template <typename T> class Mesh
{
  public:
    Mesh(std::vector<glm::vec3> &&positions, std::vector<glm::vec4> &&colors, std::vector<uint32_t> &&indices)
        : m_vertex_positions(std::move(positions)),
          m_vertex_colors(std::move(colors)),
          m_index_buffer(std::move(indices))
    {
        m_vertex_array.AddVertexAttribute(m_vertex_positions);
        m_vertex_array.AddVertexAttribute(m_vertex_colors);
    }

    void Bind() const
    {
        m_vertex_array.Bind();
        m_vertex_positions.Bind();
        m_vertex_colors.Bind();
        m_index_buffer.Bind();
    }

    void Unbind() const
    {
        m_vertex_array.Unbind();
        m_vertex_positions.Unbind();
        m_vertex_colors.Unbind();
        m_index_buffer.Unbind();
    }

    size_t NumIndices() const
    {
        return m_index_buffer.Size();
    }

  private:
    VertexAttributeBuffer<glm::vec3> m_vertex_positions;
    VertexAttributeBuffer<glm::vec4> m_vertex_colors;
    FixedIndexBuffer m_index_buffer;
    VertexArray<T> m_vertex_array;
};

} // namespace Iris
