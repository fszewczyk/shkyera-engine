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
    PropertiesWidget(std::shared_ptr<Registry> registry, const std::string& title);

    /**
     * @brief Implementation of the abstract `draw` method to render the properties widget.
     */
    void draw() override;

  protected:
    virtual void updateComponents();
    virtual void drawNewComponentMenu();
    void setupComponentsUI();

    std::shared_ptr<Registry> _registry;
    std::optional<Entity> _selectedEntity;

  private:
    void drawExistingComponents();

    std::vector<std::unique_ptr<ComponentUI>> _componentsUi;
};

class CameraPropertiesWidget : public PropertiesWidget {
  public:
    CameraPropertiesWidget(std::shared_ptr<Registry> registry);

  protected:
    void updateComponents() override;
    void drawNewComponentMenu() override;
};

class EnvironmentPropertiesWidget : public PropertiesWidget {
  public:
    EnvironmentPropertiesWidget(std::shared_ptr<Registry> registry);

  protected:
    void updateComponents() override;
    void drawNewComponentMenu() override;
};

} // namespace shkyera
