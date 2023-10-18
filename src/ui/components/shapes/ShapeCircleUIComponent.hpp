/**
 * @file ShapeCircleUIComponent.hpp
 *
 * @brief Defines the ShapeCircleUIComponent class, a specialized UIComponent for rendering circle shapes.
 */

#pragma once

#include "game/components/shapes/ShapeCircleComponent.hpp"
#include "ui/components/shapes/ShapeUIComponent.hpp"

namespace shkyera {

/**
 * @class ShapeCircleUIComponent
 *
 * @brief A specialized UIComponent for rendering circle shapes within a graphical user interface.
 *
 * The ShapeCircleUIComponent class extends the ShapeUIComponent and provides
 * methods for drawing circle shapes and handling their options. This class is
 * specific to rendering circle shapes and should be used for that purpose.
 */
class ShapeCircleUIComponent : public ShapeUIComponent {
  public:
    /**
     * @brief Constructor for ShapeCircleUIComponent.
     */
    using ShapeUIComponent::ShapeUIComponent;

    /**
     * @brief Virtual destructor for ShapeCircleUIComponent.
     */
    virtual ~ShapeCircleUIComponent() = default;

    /**
     * @brief Draw options specific to the circle shape.
     *
     * This method is responsible for rendering options specific to the circle
     * shape within the UI component.
     */
    virtual void drawOptions() override;

    /**
     * @brief Initialize the ShapeCircleUIComponent.
     *
     * Perform any initialization steps required for rendering circle shapes,
     * such as setting up resources or configuring rendering parameters.
     *
     * @param renderer A shared pointer to the Renderer used for rendering.
     */
    virtual void initialize(std::shared_ptr<Renderer> renderer) override;

  private:
    std::shared_ptr<ShapeCircleComponent> _shape; //< The circle shape associated with this component. */
};

} // namespace shkyera
