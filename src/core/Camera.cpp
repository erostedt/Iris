#include "Camera.hpp"

namespace Iris
{
    ProjectionCamera::ProjectionCamera(float fov_degrees, size_t width, size_t height, float near_clip, float far_clip)
        : m_fov_degrees(fov_degrees), m_camera_width(width), m_camera_height(height), m_near_clip(near_clip),
          m_far_clip(far_clip)
    {
        const float aspect_ratio = GetAspectRatio();
        m_view_matrix = glm::mat4(1.0f);
        m_projection_matrix = glm::perspective(glm::radians(fov_degrees), aspect_ratio, near_clip, far_clip);
    }

    float ProjectionCamera::GetAspectRatio() const
    {
        return static_cast<float>(m_camera_width) / m_camera_height;
    }

    void ProjectionCamera::LookAt(const glm::vec3 &position)
    {
        m_view_matrix = glm::lookAt(m_transform.GetPosition(), position, m_transform.GetUp());
    }

    void ProjectionCamera::MoveTo(const glm::vec3 &position)
    {
        const auto current_position = m_transform.GetPosition();
        m_transform.SetPosition(position);
        const glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position - current_position);
        m_view_matrix = m_view_matrix * glm::inverse(translation_matrix);
    }

    float ProjectionCamera::GetFovDegrees() const
    {
        return m_fov_degrees;
    }

    float ProjectionCamera::GetNearClip() const
    {
        return m_near_clip;
    }

    float ProjectionCamera::GetFarClip() const
    {
        return m_far_clip;
    }

    Transform &ProjectionCamera::GetTransform()
    {
        return m_transform;
    }

    const Transform &ProjectionCamera::GetTransform() const
    {
        return m_transform;
    }

    const glm::mat4 &ProjectionCamera::GetViewMatrix() const
    {
        return m_view_matrix;
    }

    const glm::mat4 &ProjectionCamera::GetProjectionMatrix() const
    {
        return m_projection_matrix;
    }
};
