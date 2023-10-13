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

    void drawLine(int x1, int y1, int x2, int y2);

  private:
    void initializeTexture();

    void setPixel(int x, int y, int r, int g, int b);

    uint8_t *_data = nullptr;
    uint32_t _width;
    uint32_t _height;

    std::shared_ptr<Game> _game;
    uint32_t _textureId;
};

} // namespace shkyera
