#pragma once
#include <glm/glm.hpp>

#include "Transform.hpp"

namespace Iris
{

class ProjectionCamera
{

  public:
    ProjectionCamera(float fov_degrees, size_t width, size_t height, float near_clip, float far_clip);

  public:
    float GetAspectRatio() const;
    void LookAt(const glm::vec3 &position);
    void MoveTo(const glm::vec3 &position);
    float GetFovDegrees() const;
    float GetNearClip() const;
    float GetFarClip() const;
    Transform &GetTransform();
    const Transform &GetTransform() const;
    const glm::mat4 &GetViewMatrix() const;
    const glm::mat4 &GetProjectionMatrix() const;

  private:
    float m_fov_degrees;
    uint32_t m_camera_width;
    uint32_t m_camera_height;
    float m_near_clip;
    float m_far_clip;
    Transform m_transform = Transform::Identity();
    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;
};

}; // namespace Iris
