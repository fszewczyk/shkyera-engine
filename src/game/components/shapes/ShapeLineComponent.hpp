/**
 * @file ShapeLineComponent.hpp
 *
 * @brief Defines the ShapeLineComponent class, a specialized shape component for rendering lines.
 */

#pragma once

#include "game/components/shapes/ShapeComponent.hpp"

namespace shkyera {

/**
 * @class ShapeLineComponent
 *
 * @brief A specialized shape component for rendering lines within a game or application.
 *
 * The ShapeLineComponent class is designed for rendering lines. It extends the
 * ShapeComponent class and provides methods to set the size of the line and draw it.
 */
class ShapeLineComponent final : public ShapeComponent {
  public:
    /**
     * @brief Constructor for ShapeLineComponent.
     */
    using ShapeComponent::ShapeComponent;

    /**
     * @brief Set the X-coordinate of the line's displacement.
     *
     * @param x The X-coordinate of the line's displacement.
     */
    void setX(float x);

    /**
     * @brief Set the Y-coordinate of the line's displacement.
     *
     * @param y The Y-coordinate of the line's displacement.
     */
    void setY(float y);

    /**
     * @brief Draw the line shape.
     *
     * This method is responsible for rendering the line shape using the provided
     * renderer. Subclasses should implement this method to define the drawing
     * behavior for the specific line shape.
     */
    virtual void drawShape() override;

  private:
    float _x; //< The X-coordinate of the line's displacement.
    float _y; //< The Y-coordinate of the line's displacement.
};

} // namespace shkyera
