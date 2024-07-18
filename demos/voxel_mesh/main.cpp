#include <algorithm>
#include <cstddef>
#include <glm/glm.hpp>

#include "imgui.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <stdio.h>

#include <iostream>
#include <vector>

#include "Primitives.hpp"
#include "Random.hpp"
#include "Renderer.hpp"
#include "Rotation.hpp"
#include "Shader.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;

using namespace Iris;

std::vector<ColoredVoxel> CreateColoredVoxel()
{
    std::vector<ColoredVoxel> voxels;
    const size_t xcount = 10;
    const size_t ycount = 10;
    const size_t zcount = 10;

    const float xmin = -0.5f;
    const float xmax = 0.5f;
    const float xwidth = xmax - xmin;

    const float ymin = -0.5f;
    const float ymax = 0.5f;
    const float ywidth = ymax - ymin;

    const float zmin = -0.5f;
    const float zmax = 0.5f;
    const float zwidth = zmax - zmin;

    const float dx = xwidth / xcount;
    const float dy = ywidth / ycount;
    const float dz = zwidth / zcount;

    for (size_t ix{0}; ix < xcount; ++ix)
    {
        float x = xmin + (ix + 0.5f) * dx;
        float r = (x - xmin) / xwidth;

        for (size_t iy{0}; iy < xcount; ++iy)
        {
            float y = ymin + (iy + 0.5f) * dy;
            float g = (y - ymin) / ywidth;
            for (size_t iz{0}; iz < xcount; ++iz)
            {
                float z = zmin + (iz + 0.5f) * dz;
                float b = (z - zmin) / zwidth;
                if (Random::Uniform(0.0f, 1.0f) < 0.5f)
                {
                    voxels.push_back(ColoredVoxel{Voxel{{x, y, z}, {dx, dy, dz}}, {r, g, b, 1.0}});
                }
            }
        }
    }
    return voxels;
}

int main()
{
    const auto window = SimpleWindow::Create("Triangle", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    const auto voxels = CreateColoredVoxel();
    auto voxel_mesh = CreateVoxelMesh(voxels);

    const std::string shaders_path{"../demos/voxel_mesh/shaders"};
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
            float wheel = ImGui::GetIO().MouseWheel;
            dist = std::clamp(dist + wheel, 1.0f, 10.0f);
            ImGui::End();
        }

        voxel_mesh.Bind();
        voxel_mesh.GetTransform().SetRotation({xrot, yrot, zrot});
        camera.MoveTo({0, 0, dist});
        renderer.Render(camera, voxel_mesh, shader);
    }

    return 0;
}
