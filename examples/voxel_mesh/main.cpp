#include <algorithm>
#include <cstddef>
#include <glm/glm.hpp>

#include "Texture.hpp"
#include "imgui.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
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
namespace fs = std::filesystem;

std::vector<glm::vec3> CreateVoxels()
{
    std::vector<glm::vec3> voxels;
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

    for (size_t ix = 0; ix < xcount; ++ix)
    {
        float x = xmin + (ix + 0.5f) * dx;
        for (size_t iy = 0; iy < ycount; ++iy)
        {
            float y = ymin + (iy + 0.5f) * dy;
            for (size_t iz = 0; iz < zcount; ++iz)
            {
                float z = zmin + (iz + 0.5f) * dz;
                if (Random::Uniform(0.0f, 1.0f) < 0.5f)
                {
                    voxels.push_back({x, y, z});
                }
            }
        }
    }
    return voxels;
}

int main()
{
    const auto window = Window::Create("Triangle", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    window->SetBackgroundColor(WHITE);
    const auto voxels = CreateVoxels();
    auto voxel_mesh = CreateVoxelMesh(voxels, 0.09, 0.09, 0.09);

    const fs::path shaders_path = "../examples/voxel_mesh/shaders";
    const uint32_t shader = CreateShader(shaders_path / "vertex.vert", shaders_path / "fragment.frag");

    float xrot = 0.0f;
    float yrot = 0.0f;
    float zrot = 0.0f;
    float dist = 5.0f;
    float angular_speed = 0.01f;

    const Renderer renderer;
    ProjectionCamera camera(45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);
    camera.MoveTo({0.0f, 0.0f, 5.0f});

    Texture gengar = Texture::FromFile("../gengar.png");
    gengar.Bind();

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

        voxel_mesh.GetTransform().SetRotation({xrot, yrot, zrot});
        camera.MoveTo({0, 0, dist});
        renderer.Render(camera, voxel_mesh, shader);
    }

    return 0;
}
