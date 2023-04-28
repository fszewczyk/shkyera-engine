#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>

#include "core/Image.hpp"

namespace shkyera {

Image::Image(size_t width, size_t height)
    : _assignedTextureId(false), _width(width), _height(height), _data(height, std::vector<glm::vec3>(width)) {}

Image::Image(std::filesystem::path path) : _assignedTextureId(false) {
    const char *filename = path.string().c_str();

    int bytesPerPixel = 3;
    int width, height;
    uint8_t *raw_data = stbi_load(filename, &width, &height, &bytesPerPixel, bytesPerPixel);

    if (!raw_data) {
        throw std::invalid_argument("Could not load texture from file");

        _width = 0;
        _height = 0;
        return;
    }

    if (bytesPerPixel != 3) {
        throw std::invalid_argument("Invalid bit depth");

        _width = 0;
        _height = 0;
        return;
    }

    _width = width;
    _height = height;

    _data.resize(_height);

    for (int y = 0; y < _height; ++y) {
        std::vector<glm::vec3> row(_width);

        for (int x = 0; x < _width; ++x) {
            glm::vec3 pixel;
            uint8_t *pixelOffset = raw_data + (x + y * _width) * bytesPerPixel;

            for (int channel = 0; channel < 3; ++channel) {
                pixel[channel] = static_cast<double>(pixelOffset[channel]);
            }

            row[x] = pixel;
        }
        _data[y] = row;
    }

    delete raw_data;

    // updateTextureId();
}

void Image::updateTextureId() {
    std::vector<uint8_t> data(4 * _width * _height);

    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            const glm::vec3 &c = at(x, y);
            data[(y * _width + x) * 4 + 0] = uint8_t(fabs(c[0]));
            data[(y * _width + x) * 4 + 1] = uint8_t(fabs(c[1]));
            data[(y * _width + x) * 4 + 2] = uint8_t(fabs(c[2]));
            data[(y * _width + x) * 4 + 3] = 255;
        }
    }

    unsigned textureId = _textureId;

    if (_assignedTextureId)
        glDeleteTextures(1, &textureId);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    _assignedTextureId = true;
    _textureId = textureId;
}

void Image::saveToJpg(std::filesystem::path path) const {
    char *data = new char[3 * _width * _height];

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            glm::vec3 c = at(x, y);
            data[(y * _width + x) * 3 + 0] = static_cast<int>(c[0] * 255);
            data[(y * _width + x) * 3 + 1] = static_cast<int>(c[1] * 255);
            data[(y * _width + x) * 3 + 2] = static_cast<int>(c[2] * 255);
        }
    }

    stbi_write_jpg(path.string().c_str(), _width, _height, 3, data, 100);
}

uint64_t Image::getTextureId() const { return _textureId; }

glm::vec3 Image::at(size_t x, size_t y) const { return _data[y][x]; }

Image Image::ICON_LOGO = Image("resources/icons/console/verbose.jpg");
Image Image::ICON_INFO = Image("resources/icons/console/verbose.jpg");
Image Image::ICON_ERROR = Image("resources/icons/console/verbose.jpg");
Image Image::ICON_SUCCESS = Image("resources/icons/console/verbose.jpg");
Image Image::ICON_VERBOSE = Image("resources/icons/console/verbose.jpg");

} // namespace shkyera
