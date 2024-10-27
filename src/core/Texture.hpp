#pragma once

#include "OpenGL.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <vector>

#include "Color.hpp"

namespace Iris
{

struct TextureParameters
{
    int min_filter;
    int mag_filter;
    int wrap_s;
    int wrap_t;
};

const TextureParameters DEFAULT_TEXTURE_PARAMETERS = {GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};

class Texture
{
  public:
    static Texture ColorTexture(Color color, TextureParameters parameters = DEFAULT_TEXTURE_PARAMETERS)
    {
        Texture texture;
        ByteColor c = FloatColorToByte(color);
        texture.m_data = {c.r, c.g, c.b, c.a};
        texture.m_width = 1;
        texture.m_height = 1;
        texture.m_channels = STBI_rgb_alpha;
        texture.GenerateTexture(parameters);
        return texture;
    }

    static Texture FromFile(const std::filesystem::path &filepath,
                            TextureParameters parameters = DEFAULT_TEXTURE_PARAMETERS)
    {
        assert(std::filesystem::exists(filepath));
        assert(filepath.extension().string() == ".png" || filepath.extension().string() == ".bmp");
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        uint8_t *data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        size_t byte_count = width * height * STBI_rgb_alpha;
        Texture texture;
        texture.m_data.assign(data, data + byte_count);
        texture.m_width = width;
        texture.m_height = height;
        texture.m_channels = channels;
        texture.GenerateTexture(parameters);

        stbi_image_free(data);
        return texture;
    }

    ~Texture()
    {
        glDeleteTextures(1, &m_id);
    }

    void Bind(const uint32_t texture_slot = 0)
    {
        glActiveTexture(GL_TEXTURE0 + texture_slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
private:
    void GenerateTexture(TextureParameters parameters)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameters.min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameters.mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameters.wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameters.wrap_t);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data());
        glBindTexture(GL_TEXTURE_2D, 0);
    }


  protected:
    std::vector<uint8_t> m_data;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_channels;
    uint32_t m_id;
};

}; // namespace Iris
