#include <iostream>
#include <filesystem>

#include "Directions.hpp"
#include "Primitives.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;

using namespace Iris;
namespace fs = std::filesystem;

int main()
{
    const auto window = Window::Create("Gengar", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    window->SetBackgroundColor(WHITE);

    Texture gengar = Texture::FromFile("../gengar.png");

    auto quad = CreateQuad();
    const fs::path shaders_path{"../examples/texture/shaders"};
    const uint32_t shader = CreateShader(shaders_path / "vertex.vert", shaders_path / "fragment.frag");

    const Renderer renderer;
    gengar.Bind();
    ProjectionCamera camera(45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);
    camera.MoveTo({0.0f, 0.0f, 5.0f});
    while (window->Show())
    {
        const auto frame = window->NewFrame();
        renderer.Render(camera, quad, shader);
        quad.GetTransform().RotateDegrees(1, UP);
    }
    return EXIT_SUCCESS;
}
