#include <algorithm>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "Camera.hpp"
#include "Primitives.hpp"
#include "RenderObject.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Shapes.hpp"
#include "Transform.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;
using namespace Iris;
namespace fs = std::filesystem;

glm::mat4x4 LoadProjectionMatrix(const fs::path &path)
{
    std::ifstream file(path);

    std::string line;

    const size_t rows = 3;
    const size_t cols = 4;

    glm::mat4x4 projection_matrix(1.0f);

    for (size_t row = 0; row < rows; ++row)
    {
        for (size_t col = 0; col < cols; ++col)
        {
            file >> projection_matrix[col][row];
        }
    }
    return projection_matrix;
}

std::vector<glm::mat4x4> LoadProjectionMatrices(const fs::path &directory)
{
    std::vector<glm::mat4x4> projection_matrices;
    for (size_t i = 0;; ++i)
    {
        fs::path projection_matrix_path = directory / ("david_" + std::to_string(i) + ".pa");
        if (!fs::exists(projection_matrix_path))
        {
            break;
        }
        projection_matrices.push_back(LoadProjectionMatrix(projection_matrix_path));
    }
    return projection_matrices;
}

std::vector<cv::Mat> LoadImages(const fs::path &directory)
{

    std::vector<cv::Mat> images;
    for (size_t i = 0;; ++i)
    {
        fs::path image_path = directory / ("david_" + std::to_string(i) + ".jpg");
        if (!fs::exists(image_path))
        {
            break;
        }
        images.push_back(cv::imread(image_path));
    }
    return images;
}

cv::Mat MakeMask(int width, int height, cv::Rect rect)
{
    cv::Mat mask = cv::Mat::zeros(height, width, CV_8UC1);
    mask(rect).setTo(255);
    return mask;
}

cv::Mat ExtractSilhouette(const cv::Mat &bgr_image, uint8_t threshold, cv::Rect roi)
{
    cv::Mat gray;
    cv::cvtColor(bgr_image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat bin;
    cv::threshold(gray, bin, threshold, 255, CV_8U);

    const auto mask = MakeMask(bgr_image.cols, bgr_image.rows, roi);
    cv::Mat result;
    bin.copyTo(result, mask);
    return result;
}

struct Range
{
    float start, end;
    size_t steps;
};

std::vector<Voxel> MakeVoxels(Range x, Range y, Range z)
{
    std::vector<Voxel> voxels;
    voxels.reserve(x.steps * y.steps * z.steps);

    float dx = (x.end - x.start) / x.steps;
    float dy = (y.end - y.start) / y.steps;
    float dz = (z.end - z.start) / z.steps;

    for (size_t ix = 0; ix < x.steps; ++ix)
    {
        float xp = x.start + ix * dx;
        for (size_t iy = 0; iy < y.steps; ++iy)
        {
            float yp = y.start + iy * dy;
            for (size_t iz = 0; iz < z.steps; ++iz)
            {
                float zp = z.start + iz * dz;
                voxels.push_back({glm::vec3{xp, yp, zp}, glm::vec3{dx, dy, dz}});
            }
        }
    }
    return voxels;
}

inline bool Inside(const cv::Mat &image, int x, int y)
{
    return (x >= 0) && (x < image.cols) && (y >= 0) && (y < image.rows);
}

std::vector<size_t> VoxelsHitCount(const std::vector<Voxel> &voxels, const std::vector<cv::Mat> &silhouettes,
                                   const std::vector<glm::mat4x4> &pmats)
{
    std::vector<size_t> hits(voxels.size());
    std::fill_n(std::execution::par_unseq, hits.begin(), hits.size(), 0);

    std::vector<size_t> indices(hits.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t index) {
        size_t &voxel_hits = hits[index];
        for (size_t i = 0; i < silhouettes.size(); ++i)
        {
            auto &silhouette = silhouettes.at(i);
            auto &pmat = pmats.at(i);
            glm::vec4 image_coordinates = pmat * glm::vec4(voxels[index].center, 1.0f);
            int u = image_coordinates[0] / image_coordinates[2];
            int v = image_coordinates[1] / image_coordinates[2];

            if (Inside(silhouette, u, v) && (silhouette.at<uint8_t>(v, u) == 255))
            {
                ++voxel_hits;
            }
        }
    });
    return hits;
}

std::vector<Voxel> VisualHull(std::vector<Voxel> voxels, const std::vector<cv::Mat> &silhouettes,
                              const std::vector<glm::mat4x4> &pmats, size_t min_hits)
{
    using namespace std::ranges::views;
    const auto hits = VoxelsHitCount(voxels, silhouettes, pmats);
    std::vector<Voxel> visual_hull;
    voxels.reserve(voxels.size());
    for (const auto &[voxel, hit_count] : zip(voxels, hits))
    {
        if (hit_count >= min_hits)
        {
            visual_hull.push_back(voxel);
        }
    }
    visual_hull.shrink_to_fit();
    return visual_hull;
}

std::vector<float> Bounds(const std::vector<Voxel> &voxels)
{
    float xmin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::min();

    float ymin = std::numeric_limits<float>::max();
    float ymax = std::numeric_limits<float>::min();

    float zmin = std::numeric_limits<float>::max();
    float zmax = std::numeric_limits<float>::min();

    for (const Voxel &voxel : voxels)
    {
        float x = voxel.center[0];
        float y = voxel.center[1];
        float z = voxel.center[2];
        xmin = std::min(xmin, x);
        xmax = std::max(xmax, x);

        ymin = std::min(ymin, y);
        ymax = std::max(ymax, y);

        zmin = std::min(zmin, z);
        zmax = std::max(zmax, z);
    }

    return {xmin, xmax, ymin, ymax, zmin, zmax};
}

void Centrize(std::vector<Voxel> &voxels, const std::vector<float> &bounds)
{
    float xmid = 0.5f * (bounds.at(0) + bounds.at(1));
    float ymid = 0.5f * (bounds.at(2) + bounds.at(3));
    float zmid = 0.5f * (bounds.at(4) + bounds.at(5));
    for (Voxel &voxel : voxels)
    {
        voxel.center[0] -= xmid;
        voxel.center[1] -= ymid;
        voxel.center[2] -= zmid;
    }
}

std::vector<ColoredVoxel> ColorizeVoxels(const std::vector<Voxel> &voxels, const std::vector<float> &bounds)
{
    std::vector<ColoredVoxel> cvoxels;
    for (const Voxel &voxel : voxels)
    {
        float r = (voxel.center.x - bounds[0]) / (bounds[1] - bounds[0]);
        float g = (voxel.center.y - bounds[2]) / (bounds[3] - bounds[2]);
        float b = (voxel.center.z - bounds[4]) / (bounds[5] - bounds[4]);

        cvoxels.push_back({voxel, {r, g, b, 1.0}});
    }
    return cvoxels;
}

int main()
{
    const auto window = Window::Create("Visual Hull", WINDOW_WIDTH, WINDOW_HEIGHT);

    const float xstart = 0.0;
    const float xend = 2.0;
    const size_t xsteps = 100;

    const float ystart = -1.0;
    const float yend = 3.0;
    const size_t ysteps = 200;

    const float zstart = -1.0;
    const float zend = 0.5;
    const size_t zsteps = 150;

    Range xrange{xstart, xend, xsteps};
    Range yrange{ystart, yend, ysteps};
    Range zrange{zstart, zend, zsteps};

    std::vector<Voxel> voxels = MakeVoxels(xrange, yrange, zrange);
    const auto images = LoadImages("../examples/visual_hull/data");
    const auto projection_matrices = LoadProjectionMatrices("../examples/visual_hull/data");

    std::vector<cv::Mat> silhouettes{};
    silhouettes.reserve(images.size());
    for (const cv::Mat &image : images)
    {
        silhouettes.push_back(ExtractSilhouette(image, 65, cv::Rect{100, 50, 490, 330}));
    }

    for (const cv::Mat &sil : silhouettes)
    {
        cv::imshow("Silhouettes", sil);
        cv::waitKey(100);
    }
    cv::destroyAllWindows();

    size_t threshold = 15;
    auto visual_hull = VisualHull(voxels, silhouettes, projection_matrices, threshold);

    std::vector<float> bounds = Bounds(voxels);
    Centrize(voxels, bounds);

    auto voxel_mesh = CreateVoxelMesh(ColorizeVoxels(visual_hull, bounds));

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    const fs::path shaders_path = "../examples/visual_hull/shaders";
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

        voxel_mesh.GetTransform().SetRotation({xrot, yrot, zrot});
        camera.MoveTo({0, 0, dist});
        renderer.Render(camera, voxel_mesh, shader);
    }
    return 0;
}
