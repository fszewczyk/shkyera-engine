/**
 * @file PropertiesWidget.hpp
 *
 * @brief Contains the declaration of the `PropertiesWidget` class, a user interface widget for managing game object
 * properties.
 */

#pragma once

#include <UI/Widget.hpp>
#include <UI/ComponentUI.hpp>
#include <ECS/Registry.hpp>

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

    PropertiesWidget(std::shared_ptr<Registry> registry);

    /**
     * @brief Implementation of the abstract `draw` method to render the properties widget.
     */
    void draw() override;

  private:
    void drawNewComponentMenu();

    void setupComponentsUI();

    void drawExistingComponents();

    std::optional<Entity> _selectedEntity;
    std::vector<std::unique_ptr<ComponentUI>> _componentsUi;
    std::shared_ptr<Registry> _registry;
};

} // namespace shkyera
