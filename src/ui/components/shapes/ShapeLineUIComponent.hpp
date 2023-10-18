/**
 * @file ShapeLineUIComponent.hpp
 *
 * @brief Defines the ShapeLineUIComponent class, a specialized UIComponent for rendering line shapes.
 */

#pragma once

#include "game/components/shapes/ShapeLineComponent.hpp"
#include "ui/components/shapes/ShapeUIComponent.hpp"

namespace shkyera {

/**
 * @class ShapeLineUIComponent
 *
 * @brief A specialized UIComponent for rendering line shapes within a graphical user interface.
 *
 * The ShapeLineUIComponent class extends the ShapeUIComponent and provides
 * methods for drawing line shapes and handling their options. This class is
 * specific to rendering line shapes and should be used for that purpose.
 */
class ShapeLineUIComponent : public ShapeUIComponent {
  public:
    /**
     * @brief Constructor for ShapeLineUIComponent.
     */
    using ShapeUIComponent::ShapeUIComponent;

    /**
     * @brief Virtual destructor for ShapeLineUIComponent.
     */
    virtual ~ShapeLineUIComponent() = default;

    /**
     * @brief Draw options specific to the line shape.
     *
     * This method is responsible for rendering options specific to the line
     * shape within the UI component.
     */
    virtual void drawOptions() override;

    /**
     * @brief Initialize the ShapeLineUIComponent.
     *
     * Perform any initialization steps required for rendering line shapes,
     * such as setting up resources or configuring rendering parameters.
     *
     * @param renderer A shared pointer to the Renderer used for rendering.
     */
    virtual void initialize(std::shared_ptr<Renderer> renderer) override;

  private:
    std::shared_ptr<ShapeLineComponent> _shape; /**< The line shape associated with this component. */
};

} // namespace shkyera
