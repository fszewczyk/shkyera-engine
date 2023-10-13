#include "renderer/Renderer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>

namespace shkyera {

Renderer::Renderer(std::shared_ptr<Game> game) : _game(game) { initializeTexture(); }
Renderer::~Renderer() { glDeleteTextures(1, &_textureId); }

void Renderer::draw() {
    glDeleteTextures(1, &_textureId);

    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
}

uint32_t Renderer::getTextureId() const { return _textureId; }

void Renderer::clear() { std::memset(_data, 255, 4 * _width * _height); }

void Renderer::setDimension(uint32_t width, uint32_t height) {
    if (_width == width && _height == height)
        return;

    _width = width;
    _height = height;
    if (_data != nullptr)
        delete[] _data;
    _data = new uint8_t[4 * width * height];

    clear();
}

void Renderer::drawLine(int x1, int y1, int x2, int y2) {}

void Renderer::initializeTexture() { glGenTextures(1, &_textureId); }

void Renderer::setPixel(int x, int y, int r, int g, int b) {
    int idx = 4 * (y * _width + x);
    _data[idx + 0] = r;
    _data[idx + 1] = g;
    _data[idx + 2] = b;
    _data[idx + 3] = 255;
}

} // namespace shkyera
