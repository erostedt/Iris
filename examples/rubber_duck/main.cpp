#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "Directions.hpp"
#include "ObjReader.hpp"
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
    const auto window = Window::Create("Bob", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    const fs::path example_path  = "../examples/rubber_duck";

    Texture texture = Texture::FromFile(example_path / "bob/bob_diffuse.png");
    texture.Bind();
    auto model = ReadObjFile(example_path / "bob/bob_tri.obj");

    const fs::path shaders_path = example_path / "shaders";
    const uint32_t shader = CreateShader(shaders_path / "vertex.vert", shaders_path / "fragment.frag");

    const Renderer renderer;
    ProjectionCamera camera(45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);
    camera.MoveTo({0.0f, 0.0f, 5.0f});

    while (window->Show())
    {
        const auto frame = window->NewFrame();
        renderer.Render(camera, model, shader);
        model.GetTransform().RotateDegrees(1, UP);
    }
    return EXIT_SUCCESS;
}
