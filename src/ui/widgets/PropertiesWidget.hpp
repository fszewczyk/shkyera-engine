/**
 * @file PropertiesWidget.hpp
 *
 * @brief Contains the declaration of the `PropertiesWidget` class, a user interface widget for managing game object
 * properties.
 */

#pragma once

#include "game/GameObject.hpp"
#include "renderer/Renderer.hpp"
#include "ui/UIComponent.hpp"
#include "ui/Widget.hpp"

namespace shkyera {

/**
 * @brief A user interface widget for managing game object properties.
 *
 * The `PropertiesWidget` class provides a graphical user interface for viewing and managing the properties and
 * components of a selected game object.
 */
class PropertiesWidget : public Widget {
  public:
    using Widget::Widget;

    /**
     * @brief Implementation of the abstract `draw` method to render the properties widget.
     */
    virtual void draw() override;

    /**
     * @brief Set the game object whose properties to display and manage.
     *
     * @param object A shared pointer to the game object to associate with this widget.
     */
    static void setObject(std::shared_ptr<GameObject> object);

    void setRenderer(std::shared_ptr<Renderer> renderer);

  private:
    /**
     * @brief Draw a menu for adding new components to the game object.
     */
    void drawNewComponentMenu();

    std::shared_ptr<Renderer> _renderer;

    static std::shared_ptr<GameObject> _object;                   ///< A shared pointer to the associated game object.
    static std::vector<std::shared_ptr<UIComponent>> _components; ///< Components associated with the game object.
};

} // namespace shkyera
