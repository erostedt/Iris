#include <algorithm>
#include <cstdint>
#include <cstdlib>

#include <iostream>

#include "Camera.hpp"
#include "Color.hpp"
#include "Primitives.hpp"
#include "RenderObject.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;

using namespace Iris;

int main()
{
    const auto window = Window::Create("Triangle", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    auto cube = CreateCube(RED);

    const std::string shaders_path{"../demos/cube/shaders"};
    const uint32_t shader = create_shader(shaders_path + "/vertex.vert", shaders_path + "/fragment.frag");

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

        cube.GetTransform().SetRotation({xrot, yrot, zrot});
        camera.MoveTo({0, 0, dist});
        renderer.Render(camera, cube, shader);
    }
    return 0;
}
