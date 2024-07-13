#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

#include "Rotation.hpp"

namespace Iris
{

class Transform
{
  public:
    Transform(const glm::mat4 &model_matrix) : m_model_matrix(model_matrix)
    {
    }

    static Transform Identity()
    {
        return Transform{glm::mat4(1.0f)};
    }

    const glm::mat4 &GetModelMatrix() const
    {
        return m_model_matrix;
    }

    glm::vec3 GetPosition() const
    {
        return glm::vec3(m_model_matrix[3]);
    }

    glm::quat GetQuaternion() const
    {
        return glm::quat_cast(glm::mat3(m_model_matrix));
    }

    glm::vec3 GetScale() const
    {
        return {glm::length(glm::vec3(m_model_matrix[0])), glm::length(glm::vec3(m_model_matrix[1])),
                glm::length(glm::vec3(m_model_matrix[2]))};
    }

    glm::quat GetEulerAngles() const
    {
        return glm::eulerAngles(GetQuaternion());
    }

    glm::vec3 GetRight() const
    {
        return glm::normalize(glm::vec3(m_model_matrix[0]));
    }

    glm::vec3 GetLeft() const
    {
        return -GetRight();
    }

    glm::vec3 GetUp() const
    {
        return glm::normalize(glm::vec3(m_model_matrix[1]));
    }

    glm::vec3 GetDown() const
    {
        return -GetUp();
    }

    glm::vec3 GetForward() const
    {
        return glm::normalize(glm::vec3(m_model_matrix[2]));
    }

    glm::vec3 GetBackward() const
    {
        return -GetForward();
    }

    void Translate(float x, float y, float z)
    {
        Translate({x, y, z});
    }

    void Translate(const glm::vec3 &delta)
    {
        m_model_matrix = glm::translate(m_model_matrix, delta);
    }

    void RotateRadians(float radians, const glm::vec3 &axis)
    {
        m_model_matrix = glm::rotate(m_model_matrix, UnwindRadians(radians), axis);
    }

    void RotateDegrees(float degrees, const glm::vec3 &axis)
    {
        RotateRadians(glm::radians(degrees), axis);
    }

    void Scale(const glm::vec3 &scale)
    {
        m_model_matrix = glm::scale(m_model_matrix, scale);
    }

    void Scale(float x, float y, float z)
    {
        Scale({x, y, z});
    }

    void Set(const glm::vec3 &position, const glm::vec3 &angles_radians, const glm::vec3 &scales)
    {
        m_model_matrix = glm::mat4(1.0f);
        m_model_matrix = glm::translate(m_model_matrix, position);
        m_model_matrix = glm::rotate(m_model_matrix, angles_radians.z, {1.0f, 0.0f, 0.0f});
        m_model_matrix = glm::rotate(m_model_matrix, angles_radians.y, {0.0f, 1.0f, 0.0f});
        m_model_matrix = glm::rotate(m_model_matrix, angles_radians.x, {0.0f, 0.0f, 1.0f});
        m_model_matrix = glm::scale(m_model_matrix, scales);
    }

    void SetPosition(const glm::vec3 &position)
    {
        m_model_matrix[3] = glm::vec4(position, 1.0f);
    }

    void SetRotation(const glm::vec3 &angles_radians)
    {
        auto position = GetPosition();
        auto scale = GetScale();

        m_model_matrix = glm::mat4(1.0f);
        m_model_matrix = glm::rotate(m_model_matrix, angles_radians.z, {1.0f, 0.0f, 0.0f});
        m_model_matrix = glm::rotate(m_model_matrix, angles_radians.y, {0.0f, 1.0f, 0.0f});
        m_model_matrix = glm::rotate(m_model_matrix, angles_radians.x, {0.0f, 0.0f, 1.0f});

        m_model_matrix = glm::translate(m_model_matrix, position);
        m_model_matrix = glm::scale(m_model_matrix, scale);
    }

    void SetScale(const glm::vec3 &scales)
    {
        glm::vec3 position = GetPosition();
        glm::mat3 rotation_matrix = glm::mat3(m_model_matrix);

        for (int c = 0; c < 3; ++c)
        {
            rotation_matrix[c] = glm::normalize(rotation_matrix[c]) * scales[c];
        }

        m_model_matrix = glm::mat4(m_model_matrix);
        m_model_matrix[3] = glm::vec4(position, 1.0f);
    }

  private:
    glm::mat4 m_model_matrix;
};

}; // namespace Iris
