#include <cstdlib>
#include <iostream>

#include "Directions.hpp"
#include "ObjReader.hpp"
#include "Primitives.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Window.hpp"

const size_t WINDOW_WIDTH = 1280;
const size_t WINDOW_HEIGHT = 720;

using namespace Iris;

int main()
{
    const auto window = Window::Create("F16", WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        return EXIT_FAILURE;
    }

    Texture texture = Texture::FromFile("../demos/f16/model/F16s.bmp");
    auto quad = CreateQuad();
    auto model = read_obj_file("../demos/f16/model/f16.obj");

    const std::string shaders_path{"../demos/f16/shaders"};
    const uint32_t shader = create_shader(shaders_path + "/vertex.vert", shaders_path + "/fragment.frag");

    texture.Bind();
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
