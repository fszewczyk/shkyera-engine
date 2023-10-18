/**
 * @file ShapeRectangleComponent.hpp
 *
 * @brief Defines the ShapeRectangleComponent class, a specialized shape component for rendering rectangles.
 */

#pragma once

#include "game/components/shapes/ShapeComponent.hpp"
#include <glm/glm.hpp>

namespace shkyera {

/**
 * @class ShapeRectangleComponent
 *
 * @brief A specialized shape component for rendering rectangles within a game or application.
 *
 * The ShapeRectangleComponent class is designed for rendering rectangle shapes. It extends
 * the ShapeComponent class and provides methods to set the dimensions and color of the rectangle
 * and to draw it.
 */
class ShapeRectangleComponent final : public ShapeComponent {
  public:
    /**
     * @brief Constructor for ShapeRectangleComponent.
     */
    using ShapeComponent::ShapeComponent;

    /**
     * @brief Set the width of the rectangle.
     *
     * @param w The width of the rectangle.
     */
    void setWidth(float w);

    /**
     * @brief Set the height of the rectangle.
     *
     * @param h The height of the rectangle.
     */
    void setHeight(float h);

    /**
     * @brief Set the color of the rectangle.
     *
     * @param color A glm::vec3 representing the color of the rectangle.
     */
    void setColor(glm::vec3 color);

    /**
     * @brief Draw the rectangle shape.
     *
     * This method is responsible for rendering the rectangle shape using the provided
     * renderer. Subclasses should implement this method to define the drawing behavior
     * for the specific rectangle shape.
     */
    virtual void drawShape() override;

  private:
    glm::vec3 _color; //< The color of the rectangle.
    float _width;     //< The width of the rectangle.
    float _height;    //< The height of the rectangle.
};

} // namespace shkyera
