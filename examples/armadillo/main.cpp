#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "Octree.hpp"
#include "Camera.hpp"
#include "PlyReader.hpp"
#include "Primitives.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Shapes.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;
namespace fs = std::filesystem;

using namespace Iris;

BoundingBox bounding_box(const std::vector<glm::vec3> &points)
{
    float minx = points.front().x;
    float maxx = points.front().x;

    float miny = points.front().y;
    float maxy = points.front().y;

    float minz = points.front().z;
    float maxz = points.front().z;

    for (const glm::vec3 &point : points)
    {
        minx = std::min(minx, point.x);
        maxx = std::max(maxx, point.x);

        miny = std::min(miny, point.y);
        maxy = std::max(maxy, point.y);

        minz = std::min(minz, point.z);
        maxz = std::max(maxz, point.z);
    }

    return {{minx, miny, minz}, {maxx, maxy, maxz}};
}

int main()
{
    const auto window = Window::Create("Armadillo", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    const fs::path armadillo_path = "../examples/armadillo/data/armadillo.ply";
    auto points = PointCloud3dFromPly(armadillo_path);
    if (!points)
    {
        std::cout << "Could not parse file" << std::endl;
        return EXIT_FAILURE;
    }

    const fs::path shaders_path = "../examples/armadillo/shaders";
    uint32_t shader = CreateShader(shaders_path / "vertex.vert", shaders_path / "fragment.frag");

    BoundingBox bounds = bounding_box(*points).padded(1.0001f, 1.0001f, 1.0001f);

    std::vector<ColoredSphere> spheres;
    spheres.reserve(points->size());
    for (const auto &point : *points)
    {
        glm::vec3 normalized_pos = (point - bounds.min) / bounds.dimensions();
        glm::vec3 shifted = point;
        spheres.push_back({{shifted, 0.05f}, {normalized_pos, 1.0f}});
    }
    auto point_cloud = CreatePointCloud(spheres, 5, 10);

    float xrot = 0.0f;
    float yrot = glm::pi<float>();
    float zrot = 0.0f;
    float dist = 300.0f;
    float angular_speed = 0.01f;

    const Renderer renderer;
    ProjectionCamera camera(45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 1000.0f);
    camera.MoveTo({0.0f, 0.0f, 5.0f});

    while (window->Show())
    {
        const auto frame = window->NewFrame();

        {
            if (ImGui::GetIO().MouseDown[1])
            {
                xrot = UnwindRadians(xrot + angular_speed * ImGui::GetIO().MouseDelta.y);
                yrot = UnwindRadians(yrot + angular_speed * ImGui::GetIO().MouseDelta.x);
            }
            const float wheel = ImGui::GetIO().MouseWheel;
            dist = std::clamp(dist + wheel, -1000.0f, 1000.0f);
        }

        point_cloud.GetTransform().SetRotation({xrot, yrot, zrot});
        camera.MoveTo({0, 0, dist});
        renderer.Render(camera, point_cloud, shader);
    }
    return 0;
}
