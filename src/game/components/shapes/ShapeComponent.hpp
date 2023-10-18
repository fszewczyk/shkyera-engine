/**
 * @file ShapeComponent.hpp
 *
 * @brief Defines the ShapeComponent class, a base component for handling shapes.
 */

#pragma once

#include "game/Component.hpp"
#include "renderer/Renderer.hpp"
#include <vector>

namespace shkyera {

/**
 * @class ShapeComponent
 *
 * @brief A base component for handling shapes within a game or application.
 *
 * The ShapeComponent class is an abstract base class that defines methods for handling
 * shapes and managing rendering. Specific shape components should derive from this class
 * to implement their own drawing logic.
 */
class ShapeComponent : public Component {
  public:
    /**
     * @brief Constructor for ShapeComponent.
     *
     * @param parent The parent Component, if any.
     */
    using Component::Component;

    /**
     * @brief Set the renderer for this shape component.
     *
     * @param renderer A shared pointer to the Renderer used for rendering.
     */
    void setRenderer(std::shared_ptr<Renderer> renderer);

    /**
     * @brief Draw the shape.
     *
     * This pure virtual method should be implemented by subclasses to define the
     * drawing behavior for their specific shape type.
     */
    virtual void drawShape() = 0;

    /**
     * @brief Add a shape component to the list of managed shapes.
     *
     * @param shape A shared pointer to the ShapeComponent to be added.
     */
    static void addShape(std::shared_ptr<ShapeComponent> shape);

    /**
     * @brief Remove a shape component from the list of managed shapes.
     *
     * @param shape A shared pointer to the ShapeComponent to be removed.
     */
    static void removeShape(std::shared_ptr<ShapeComponent> shape);

    /**
     * @brief Get the list of managed shape components.
     *
     * @return A vector of shared pointers to ShapeComponent representing the managed shapes.
     */
    static std::vector<std::shared_ptr<ShapeComponent>> getShapes();

  protected:
    std::shared_ptr<Renderer> _renderer; //< The renderer used for rendering the shape.

  private:
    static std::vector<std::shared_ptr<ShapeComponent>> _shapes; //< List of managed shape components.
};

} // namespace shkyera
