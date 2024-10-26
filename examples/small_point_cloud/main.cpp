#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <filesystem>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Renderer.hpp"
#include "Rotation.hpp"
#include "imgui.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <alloca.h>
#include <iostream>
#include <vector>

#include "Primitives.hpp"
#include "Shader.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;

using namespace Iris;
namespace fs = std::filesystem;

std::vector<ColoredSphere> CreateSpheres()
{
    return {

        {Sphere{{-1.0f, -1.0f, -1.0f}, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {Sphere{{-1.0f, -1.0f, 1.0f}, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {Sphere{{-1.0f, 1.0f, -1.0f}, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        {Sphere{{-1.0f, 1.0f, 1.0f}, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {Sphere{{1.0f, -1.0f, -1.0f}, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {Sphere{{1.0f, -1.0f, 1.0f}, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        {Sphere{{1.0f, 1.0f, -1.0f}, 0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {Sphere{{1.0f, 1.0f, 1.0f}, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}}

    };
}

int main()
{
    const auto window = Window::Create("Triangle", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    const auto spheres = CreateSpheres();
    auto point_cloud = CreatePointCloud(spheres);

    const fs::path shaders_path = "../examples/small_point_cloud/shaders";
    const uint32_t shader = CreateShader(shaders_path / "vertex.vert", shaders_path / "fragment.frag");

    float xrot = 0.0f;
    float yrot = 0.0f;
    float zrot = 0.0f;
    float dist = 5.0f;
    float angular_speed = 0.01f;

    const Renderer renderer;
    ProjectionCamera camera(45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);
    camera.MoveTo({0.0f, 0.0f, 5.0f});

    while (window->Show())
    {
        const auto frame = window->NewFrame();

        {
            ImGui::Begin("Controls");
            if (ImGui::GetIO().MouseDown[1])
            {
                xrot = UnwindRadians(xrot + angular_speed * ImGui::GetIO().MouseDelta.y);
                yrot = UnwindRadians(yrot + angular_speed * ImGui::GetIO().MouseDelta.x);
            }
            const float wheel = ImGui::GetIO().MouseWheel;
            dist = std::clamp(dist + wheel, 1.0f, 10.0f);
            ImGui::End();
        }

        point_cloud.GetTransform().SetRotation({xrot, yrot, zrot});
        camera.MoveTo({0, 0, dist});
        renderer.Render(camera, point_cloud, shader);
    }

    return 0;
}
