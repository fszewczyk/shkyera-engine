#include "renderer/Renderer.hpp"

#include <fstream>
#include <iostream>
#include <mutex>
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
    std::memcpy(_deployData, _data, 4 * _width * _height);

    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _deployData);
}

uint32_t Renderer::getTextureId() const { return _textureId; }

void Renderer::clear() { std::memset(_data, 255, 4 * _width * _height); }

void Renderer::setDimension(uint32_t width, uint32_t height) {
    if (_width == width && _height == height)
        return;

    _width = width;
    _height = height;

    if (_data != nullptr) {
        delete[] _data;
        delete[] _deployData;
    }
    _data = new uint8_t[4 * width * height];
    _deployData = new uint8_t[4 * width * height];

    clear();
    std::memcpy(_deployData, _data, 4 * _width * _height);
}

void Renderer::drawLine(int x0, int y0, int x1, int y1) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1)
            drawLineLow(x1, y1, x0, y0);
        else
            drawLineLow(x0, y0, x1, y1);
    } else {
        if (y0 > y1)
            drawLineHigh(x1, y1, x0, y0);
        else
            drawLineHigh(x0, y0, x1, y1);
    }
}

void Renderer::drawLineLow(int x0, int y0, int x1, int y1) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float yi = 1;
    if (dy < 0) {
        yi = -1;
        dy *= -1;
    }

    float D = 2 * dy - dx;
    float y = y0;

    for (float x = x0; x <= x1; x++) {
        int xCoord = (int)x;
        int yCoord = (int)y;
        if (xCoord >= 0 && xCoord < _width && yCoord >= 0 && yCoord < _height)
            setPixel(xCoord, yCoord, 0, 0, 0);

        if (D > 0) {
            y += yi;
            D += 2 * (dy - dx);
        } else {
            D += 2 * dy;
        }
    }
}

void Renderer::drawLineHigh(int x0, int y0, int x1, int y1) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float xi = 1;
    if (dx < 0) {
        xi = -1;
        dx *= -1;
    }
    float D = 2 * dx - dy;
    float x = x0;

    for (float y = y0; y <= y1; y++) {
        int xCoord = (int)x;
        int yCoord = (int)y;
        if (xCoord >= 0 && xCoord < _width && yCoord >= 0 && yCoord < _height)
            setPixel(xCoord, yCoord, 0, 0, 0);

        if (D > 0) {
            x += xi;
            D += 2 * (dx - dy);
        } else {
            D += 2 * dx;
        }
    }
}

void Renderer::initializeTexture() { glGenTextures(1, &_textureId); }

void Renderer::setPixel(int x, int y, int r, int g, int b) {
    int idx = 4 * (y * _width + x);
    _data[idx + 0] = r;
    _data[idx + 1] = g;
    _data[idx + 2] = b;
    _data[idx + 3] = 255;
}

} // namespace shkyera
