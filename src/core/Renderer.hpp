#pragma once
#include "OpenGL.hpp"

#include <glm/glm.hpp>

#include "Camera.hpp"
#include "RenderObject.hpp"

namespace Iris
{

class Renderer
{
  public:
    template <typename T>
    void Render(const ProjectionCamera &camera, const RenderObject<T> &render_object, const uint32_t shader) const
    {
        const auto projection_matrix = camera.GetProjectionMatrix();
        const auto view_matrix = camera.GetViewMatrix();
        const auto model_matrix = render_object.GetModelMatrix();

        render_object.Bind();
        const Mesh<T> &mesh = render_object.GetMesh();

        const int model_loc = glGetUniformLocation(shader, "u_model");
        assert(model_loc != -1);

        const int view_loc = glGetUniformLocation(shader, "u_view");
        assert(view_loc != -1);

        const int proj_loc = glGetUniformLocation(shader, "u_projection");
        assert(proj_loc != -1);

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glUseProgram(shader);
        glDrawElements(GL_TRIANGLES, mesh.NumIndices(), GL_UNSIGNED_INT, nullptr);
    }
};

}; // namespace Iris
