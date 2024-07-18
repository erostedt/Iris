#pragma once
#include "OpenGL.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.hpp"

namespace Iris
{

class ProjectionCamera
{

  public:
    ProjectionCamera(float fov_degrees, size_t width, size_t height, float near_clip, float far_clip)
        : m_fov_degrees(fov_degrees), m_camera_width(width), m_camera_height(height), m_near_clip(near_clip),
          m_far_clip(far_clip)
    {
        const float aspect_ratio = GetAspectRatio();
        m_view_matrix = glm::mat4(1.0f);
        m_projection_matrix = glm::perspective(glm::radians(fov_degrees), aspect_ratio, near_clip, far_clip);
    }

  public:
    float GetAspectRatio() const
    {
        return static_cast<float>(m_camera_width) / m_camera_height;
    }

    void LookAt(const glm::vec3 &position)
    {
        m_view_matrix = glm::lookAt(m_transform.GetPosition(), position, m_transform.GetUp());
    }

    void MoveTo(const glm::vec3 &position)
    {
        const auto current_position = m_transform.GetPosition();
        m_transform.SetPosition(position);
        const glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position - current_position);
        m_view_matrix = m_view_matrix * glm::inverse(translation_matrix);
    }

    float GetFovDegrees() const
    {
        return m_fov_degrees;
    }

    float GetNearClip() const
    {
        return m_near_clip;
    }

    float GetFarClip() const
    {
        return m_far_clip;
    }

    Transform &GetTransform()
    {
        return m_transform;
    }

    const Transform &GetTransform() const
    {
        return m_transform;
    }

    const glm::mat4 &GetViewMatrix() const
    {
        return m_view_matrix;
    }

    const glm::mat4 &GetProjectionMatrix() const
    {
        return m_projection_matrix;
    }

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
