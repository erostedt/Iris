#include <cstdlib>
#include <filesystem>

#include "Primitives.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;

using namespace Iris;
namespace fs = std::filesystem;

int main()
{
    const auto window = Window::Create("Triangle", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    auto triangle = CreateTriangle(RED);
    Texture texture = Texture::ColorTexture(GREEN);
    texture.Bind();
    const fs::path shaders_path = "../examples/triangle/shaders";
    const uint32_t shader = create_shader(shaders_path / "vertex.vert", shaders_path / "fragment.frag");

    const Renderer renderer;
    ProjectionCamera camera(45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);
    camera.MoveTo({0.0f, 0.0f, 5.0f});
    while (window->Show())
    {
        const auto frame = window->NewFrame();
        renderer.Render(camera, triangle, shader);
    }
    return EXIT_SUCCESS;
}
