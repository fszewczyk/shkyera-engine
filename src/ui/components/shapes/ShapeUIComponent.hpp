/**
 * @file ShapeUIComponent.hpp
 *
 * @brief Defines the ShapeUIComponent class, a UIComponent for rendering shapes.
 */

#pragma once

#include "game/components/shapes/ShapeComponent.hpp"
#include "ui/UIComponent.hpp"

namespace shkyera {

/**
 * @class ShapeUIComponent
 *
 * @brief A specialized UIComponent for rendering shapes within a graphical user interface.
 *
 * The ShapeUIComponent class extends the basic UIComponent class and provides
 * methods for drawing shapes and handling their options. This class should be
 * further subclassed to implement specific shapes and their drawing logic.
 */
class ShapeUIComponent : public UIComponent {
  public:
    /**
     * @brief Constructor for ShapeUIComponent.
     */
    using UIComponent::UIComponent;

    /**
     * @brief Virtual destructor for ShapeUIComponent.
     */
    virtual ~ShapeUIComponent() = default;

    /**
     * @brief Draw the shape within the UI component.
     *
     * This method is responsible for rendering the UI component.
     * Subclasses should implement this method to define the drawing behavior for
     * their specific shape type.
     */
    virtual void draw() override;

    /**
     * @brief Draw options specific to the shape.
     *
     * This pure virtual method should be implemented by subclasses to draw
     * shape-specific options within the user interface, if applicable.
     */
    virtual void drawOptions() = 0;

    /**
     * @brief Initialize the ShapeUIComponent.
     *
     * Subclasses should implement this method to perform any initialization
     * steps required for their specific shape rendering. In particular, they should assign
     * the renderer.
     *
     * @param renderer A shared pointer to the Renderer used for rendering.
     */
    virtual void initialize(std::shared_ptr<Renderer> renderer) = 0;
};

} // namespace shkyera
