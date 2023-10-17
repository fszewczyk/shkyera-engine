#pragma once

#include "game/Game.hpp"

namespace shkyera {

/**
 * @class Renderer
 * @brief A class for rendering graphics in a game.
 */
class Renderer {
  public:
    /**
     * @brief Constructor for the Renderer class.
     *
     * @param game A shared pointer to the Game instance to associate with the renderer.
     */
    Renderer(std::shared_ptr<Game> game);

    /**
     * @brief Destructor for the Renderer class.
     */
    ~Renderer();

    /**
     * @brief Render the graphics.
     */
    void draw();

    /**
     * @brief Set the dimensions of the rendering area.
     *
     * @param width The width of the rendering area.
     * @param height The height of the rendering area.
     */
    void setDimension(uint32_t width, uint32_t height);

    /**
     * @brief Get the texture ID associated with the renderer.
     *
     * @return The texture ID.
     */
    uint32_t getTextureId() const;

    /**
     * @brief Clear the rendering area by setting all pixels to white.
     */
    void clear();

    /**
     * @brief Draw a line from (x0, y0) to (x1, y1).
     *
     * @param x0 The x-coordinate of the starting point.
     * @param y0 The y-coordinate of the starting point.
     * @param x1 The x-coordinate of the ending point.
     * @param y1 The y-coordinate of the ending point.
     */
    void drawLine(int x0, int y0, int x1, int y1);

    /**
     * @brief Draw a circle at (x, y) with radius r.
     *
     * @param x The x-coordinate of the circle.
     * @param y The y-coordinate of the circle.
     * @param r The radius of the circle.
     */
    void drawCircle(int x, int y, int r);

  private:
    /**
     * @brief Initialize the texture used for rendering.
     */
    void initializeTexture();

    /**
     * @brief Draw a line with a low slope.
     */
    void drawLineLow(int x0, int y0, int x1, int y1);

    /**
     * @brief Draw a line with a high slope.
     */
    void drawLineHigh(int x0, int y0, int x1, int y1);

    /**
     * @brief Set the color of a pixel at (x, y).
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param r The red component of the color.
     * @param g The green component of the color.
     * @param b The blue component of the color.
     */
    void setPixel(int x, int y, int r, int g, int b);

    uint8_t *_data = nullptr;       ///< A pointer to the pixel data.
    uint8_t *_deployData = nullptr; ///< A pointer to the deployed pixel data.
    uint32_t _width;                ///< The width of the rendering area.
    uint32_t _height;               ///< The height of the rendering area.

    std::shared_ptr<Game> _game; ///< A shared pointer to the associated Game instance.
    uint32_t _textureId;         ///< The texture ID used for rendering.
};

} // namespace shkyera
