/**
 * @file ShapeCircleComponent.hpp
 *
 * @brief Defines the ShapeCircleComponent class, a specialized shape component for rendering circles.
 */

#pragma once

#include "game/components/shapes/ShapeComponent.hpp"

namespace shkyera {

/**
 * @class ShapeCircleComponent
 *
 * @brief A specialized shape component for rendering circles within a game or application.
 *
 * The ShapeCircleComponent class is designed for rendering circle shapes. It extends
 * the ShapeComponent class and provides methods to set the radius of the circle and
 * to draw it.
 */
class ShapeCircleComponent final : public ShapeComponent {
  public:
    /**
     * @brief Constructor for ShapeCircleComponent.
     *
     * @param parent The parent Component, if any.
     */
    using ShapeComponent::ShapeComponent;

    /**
     * @brief Set the radius of the circle.
     *
     * @param r The radius of the circle.
     */
    void setRadius(float r);

    /**
     * @brief Draw the circle shape.
     *
     * This method is responsible for rendering the circle shape using the provided
     * renderer. Subclasses should implement this method to define the drawing behavior
     * for the specific circle shape.
     */
    virtual void drawShape() override;

  private:
    float _radius; //< The radius of the circle.
};

} // namespace shkyera
