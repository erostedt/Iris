#include "OpenGL.hpp"

#include <algorithm>
#include <execution>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "Camera.hpp"
#include "Color.hpp"
#include "Primitives.hpp"
#include "RenderObject.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Shapes.hpp"
#include "Transform.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;
namespace fs = std::filesystem;

using namespace Iris;

cv::Mat LoadProjectionMatrix(const fs::path &path)
{
    std::ifstream file(path);

    std::string line;

    const size_t rows = 3;
    const size_t cols = 4;

    cv::Mat projection_matrix(rows, cols, CV_32F);
    const size_t n_elements = projection_matrix.total();

    size_t idx = 0;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        float elem;
        while ((iss >> elem) && (idx < n_elements))
        {
            projection_matrix.at<float>(idx) = elem;
            ++idx;
        }
    }
    return projection_matrix;
}


std::vector<cv::Mat> LoadProjectionMatrices(const fs::path& directory)
{
    std::vector<cv::Mat> projection_matrices;
    for (size_t i = 0;; ++i)
    {
        fs::path projection_matrix_path = directory / ("david_" + std::to_string(i) + ".pa");
        if (!fs::exists(projection_matrix_path))
        {
            break;
        }
        projection_matrices.push_back(LoadProjectionMatrix(projection_matrix_path));
    }
    std::cout << projection_matrices.size() << std::endl;
    return projection_matrices;
}

std::vector<cv::Mat> LoadImages(const fs::path& directory)
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
    std::cout << images.size() << std::endl;
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
    std::cout << bin.rows << " " << bin.cols << std::endl;
    std::cout << mask.rows << " " << mask.cols << std::endl;
    cv::Mat result;
    bin.copyTo(result, mask);
    return result;
}

struct DVoxel
{
    cv::Mat h_coords;
    size_t hits;
};

struct Range
{
    float start, end;
    size_t steps;
};

std::vector<DVoxel> make_voxels(Range x, Range y, Range z)
{
    std::vector<DVoxel> voxels{};
    voxels.reserve(x.steps * y.steps * z.steps);

    float dx = (x.end - x.start) / x.steps;
    float dy = (y.end - y.start) / y.steps;
    float dz = (z.end - z.start) / z.steps;

    for (size_t ix{0}; ix < x.steps; ix++)
    {
        float xp = x.start + ix * dx;
        for (size_t iy{0}; iy < y.steps; iy++)
        {
            float yp = y.start + iy * dy;
            for (size_t iz{0}; iz < z.steps; iz++)
            {
                float zp = z.start + iz * dz;
                cv::Mat h_coord = cv::Mat::zeros(4, 1, CV_32F);
                h_coord.at<float>(0) = xp;
                h_coord.at<float>(1) = yp;
                h_coord.at<float>(2) = zp;
                h_coord.at<float>(3) = 1.0;

                voxels.push_back(DVoxel{h_coord, 0});
            }
        }
    }
    return voxels;
}

inline bool inside(const cv::Mat &image, int x, int y)
{
    return (x >= 0) && (x < image.cols) && (y >= 0) && (y < image.rows);
}

void visual_hull(std::vector<DVoxel> &voxels, const std::vector<cv::Mat> &silhouettes, const std::vector<cv::Mat> &pmats)
{
    size_t n_samples = silhouettes.size();
    std::for_each(std::execution::par, voxels.begin(), voxels.end(), [&](DVoxel &voxel) {
        for (size_t i{0}; i < n_samples; i++)
        {
            auto &silhouette = silhouettes.at(i);
            auto &pmat = pmats.at(i);
            cv::Mat image_coordinates = pmat * voxel.h_coords;
            int u = image_coordinates.at<float>(0) / image_coordinates.at<float>(2);
            int v = image_coordinates.at<float>(1) / image_coordinates.at<float>(2);

            if (inside(silhouette, u, v) && (silhouette.at<uint8_t>(v, u) == 255))
            {
                voxel.hits++;
            }
        }
    });
}

std::vector<float> bounds(const std::vector<DVoxel> &voxels)
{

    float xmin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::min();

    float ymin = std::numeric_limits<float>::max();
    float ymax = std::numeric_limits<float>::min();

    float zmin = std::numeric_limits<float>::max();
    float zmax = std::numeric_limits<float>::min();

    for (const DVoxel &voxel : voxels)
    {
        float x = voxel.h_coords.at<float>(0);
        float y = voxel.h_coords.at<float>(1);
        float z = voxel.h_coords.at<float>(2);
        xmin = std::min(xmin, x);
        xmax = std::max(xmax, x);

        ymin = std::min(ymin, y);
        ymax = std::max(ymax, y);

        zmin = std::min(zmin, z);
        zmax = std::max(zmax, z);
    }

    return {xmin, xmax, ymin, ymax, zmin, zmax};
}

void centrize(std::vector<DVoxel> &voxels, const std::vector<float> &bounds)
{
    float xmid = 0.5f * (bounds.at(0) + bounds.at(1));
    float ymid = 0.5f * (bounds.at(2) + bounds.at(3));
    float zmid = 0.5f * (bounds.at(4) + bounds.at(5));
    for (DVoxel &voxel : voxels)
    {
        voxel.h_coords.at<float>(0) -= xmid;
        voxel.h_coords.at<float>(1) -= ymid;
        voxel.h_coords.at<float>(2) -= zmid;
    }
}


int main()
{
    const auto window = Window::Create("Visual Hull", WINDOW_WIDTH, WINDOW_HEIGHT);

    const float xstart = 0.0;
    const float xend = 2.0;

    const float ystart = -1.0;
    const float yend = 3.0;

    const float zstart = -1.0;
    const float zend = 0.5;

    const float scale = 50.0;

    size_t nx = scale * (xend - xstart);
    size_t ny = scale * (yend - ystart);
    size_t nz = scale * (zend - zstart);
    const float dx = (xend - xstart) / nx;
    const float dy = (yend - ystart) / ny;
    const float dz = (zend - zstart) / nz;

    Range xrange{xstart, xend, nx};
    Range yrange{ystart, yend, ny};
    Range zrange{zstart, zend, nz};

    std::vector<DVoxel> voxels = make_voxels(xrange, yrange, zrange);
    const auto images = LoadImages("../demos/david/data");
    const auto projection_matrices = LoadProjectionMatrices("../demos/david/data");

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

    visual_hull(voxels, silhouettes, projection_matrices);
    std::vector<float> bnds = bounds(voxels);
    centrize(voxels, bnds);

    size_t threshold = 15;
    std::vector<ColoredVoxel> cvoxels;
    for (const DVoxel &voxel : voxels)
    {
        if (voxel.hits >= threshold)
        {
            float x = voxel.h_coords.at<float>(0);
            float y = voxel.h_coords.at<float>(1);
            float z = voxel.h_coords.at<float>(2);

            float r = (x - xstart) / (xend - xstart);
            float g = (y - ystart) / (yend - ystart);
            float b = (z - zstart) / (zend - zstart);

            cvoxels.push_back({{{x, y, z}, {dx, dy, dz}}, {r, g, b, 1.0}});
        }
    }
    std::cout << cvoxels.size() << std::endl;
    auto voxel_mesh = CreateVoxelMesh(cvoxels);


    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string shaders_path{"../demos/david/shaders"};
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

        voxel_mesh.GetTransform().SetRotation({xrot, yrot, zrot});
        camera.MoveTo({0, 0, dist});
        renderer.Render(camera, voxel_mesh, shader);
    }
    return 0;
}
