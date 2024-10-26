#pragma once

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

namespace Iris
{

class Mesh
{
  public:
    Mesh(std::vector<glm::vec3> &&positions, std::vector<glm::vec3> &&normals, std::vector<glm::vec2> &&texture_coordinates, std::vector<uint32_t> &&indices)
        : m_vertex_positions(std::move(positions)),
          m_vertex_normals(std::move(normals)),
          m_vertex_texture_coordinates(std::move(texture_coordinates)),
          m_index_buffer(std::move(indices))
    {
        m_vertex_array.AddVertexAttribute(m_vertex_positions);
        m_vertex_array.AddVertexAttribute(m_vertex_normals);
        m_vertex_array.AddVertexAttribute(m_vertex_texture_coordinates);
    }

    void Bind() const
    {
        m_vertex_array.Bind();
        m_vertex_positions.Bind();
        m_vertex_normals.Bind();
        m_vertex_texture_coordinates.Bind();
        m_index_buffer.Bind();
    }

    void Unbind() const
    {
        m_vertex_array.Unbind();
        m_vertex_positions.Unbind();
        m_vertex_normals.Unbind();
        m_vertex_texture_coordinates.Unbind();
        m_index_buffer.Unbind();
    }

    size_t NumIndices() const
    {
        return m_index_buffer.Size();
    }

  private:
    VertexAttributeBuffer<glm::vec3> m_vertex_positions;
    VertexAttributeBuffer<glm::vec3> m_vertex_normals;
    VertexAttributeBuffer<glm::vec2> m_vertex_texture_coordinates;
    IndexBuffer m_index_buffer;
    VertexArray m_vertex_array;
};

} // namespace Iris
