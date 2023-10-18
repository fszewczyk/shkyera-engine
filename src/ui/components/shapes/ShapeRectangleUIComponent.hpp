/**
 * @file ShapeRectangleUIComponent.hpp
 *
 * @brief Defines the ShapeRectangleUIComponent class, a specialized UIComponent for rendering rectangle shapes.
 */

#pragma once

#include "game/components/shapes/ShapeRectangleComponent.hpp"
#include "ui/components/shapes/ShapeUIComponent.hpp"

namespace shkyera {

/**
 * @class ShapeRectangleUIComponent
 *
 * @brief A specialized UIComponent for rendering rectangle shapes within a graphical user interface.
 *
 * The ShapeRectangleUIComponent class extends the ShapeUIComponent and provides
 * methods for drawing rectangle shapes and handling their options. This class is
 * specific to rendering rectangle shapes and should be used for that purpose.
 */
class ShapeRectangleUIComponent : public ShapeUIComponent {
  public:
    /**
     * @brief Constructor for ShapeRectangleUIComponent.
     */
    using ShapeUIComponent::ShapeUIComponent;

    /**
     * @brief Virtual destructor for ShapeRectangleUIComponent.
     */
    virtual ~ShapeRectangleUIComponent() = default;

    /**
     * @brief Draw options specific to the rectangle shape.
     *
     * This method is responsible for rendering options specific to the rectangle
     * shape within the UI component.
     */
    virtual void drawOptions() override;

    /**
     * @brief Initialize the ShapeRectangleUIComponent.
     *
     * Perform any initialization steps required for rendering rectangle shapes,
     * such as setting up resources or configuring rendering parameters.
     *
     * @param renderer A shared pointer to the Renderer used for rendering.
     */
    virtual void initialize(std::shared_ptr<Renderer> renderer) override;

  private:
    std::shared_ptr<ShapeRectangleComponent> _shape; //< The rectangle shape associated with this component. */
};

} // namespace shkyera
