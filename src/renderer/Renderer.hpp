#pragma once

#include "game/Game.hpp"

namespace shkyera {

class Renderer {
  public:
    Renderer(std::shared_ptr<Game> game);
    ~Renderer();

    void draw();
    void setDimension(uint32_t width, uint32_t height);

    uint32_t getTextureId() const;

    void clear();

    void drawLine(int x0, int y0, int x1, int y1);

  private:
    void initializeTexture();

    // Line Drawing Helper Functions
    void drawLineLow(int x0, int y0, int x1, int y1);
    void drawLineHigh(int x0, int y0, int x1, int y1);

    void setPixel(int x, int y, int r, int g, int b);

    uint8_t *_data = nullptr;
    uint8_t *_deployData = nullptr;
    uint32_t _width;
    uint32_t _height;

    std::shared_ptr<Game> _game;
    uint32_t _textureId;
};

} // namespace shkyera
