#pragma once

#include <memory>

#include "Mesh.hpp"
#include "Transform.hpp"

namespace Iris
{

template <typename T> class RenderObject
{
  public:
    RenderObject(std::unique_ptr<Mesh<T>> &&mesh, const Transform &transform)
        : m_mesh(std::move(mesh)), m_transform(transform)
    {
    }
    RenderObject(std::unique_ptr<Mesh<T>> &&mesh) : m_mesh(std::move(mesh))
    {
    }
    const glm::mat4 &GetModelMatrix() const
    {
        return m_transform.GetModelMatrix();
    }

    const Mesh<T> &GetMesh() const
    {
        return *m_mesh;
    }

    const void Bind() const
    {
        m_mesh->Bind();
        // Mateiral bind
    }

    Transform &GetTransform()
    {
        return m_transform;
    }

  protected:
    std::unique_ptr<Mesh<T>> m_mesh;
    Transform m_transform = Transform::Identity();
    // Mateiral
};

}; // namespace Iris
