/**
 * @file UIComponent.hpp
 *
 * @brief Contains the declaration of the `UIComponent` class, a base class for user interface of objects' components.
 */

#pragma once

#include <unordered_map>

#include "game/GameObject.hpp"

namespace shkyera {

/**
 * @brief A base class representing a user interface (UI) for a game object's component.
 *
 * The `UIComponent` class defines the common interface for UI object components and provides functionality
 * for managing and rendering them within the app.
 */
class UIComponent {
  public:
    /**
     * @brief Constructor to create a UI component with a specified name and associated game object.
     *
     * @param name The name of the UI component.
     * @param object A shared pointer to the game object associated with the UI component.
     */
    UIComponent(std::string name, std::shared_ptr<GameObject> object);

    /**
     * @brief Abstract method to draw the UI component.
     *
     * This method must be implemented by derived classes to render the UI component on the screen.
     */
    virtual void draw() = 0;

    /**
     * @brief Static method to add a UI component to a game object.
     *
     * @param object A shared pointer to the game object.
     * @param component A shared pointer to the UI component to add.
     */
    static void addComponentToObject(std::shared_ptr<GameObject> object, std::shared_ptr<UIComponent> component);

    /**
     * @brief Static method to retrieve a list of UI components associated with a game object.
     *
     * @param object A shared pointer to the game object.
     * @return A vector of shared pointers to UI components associated with the game object.
     */
    static std::vector<std::shared_ptr<UIComponent>> getComponentsOfObject(std::shared_ptr<GameObject> object);

  protected:
    uint64_t _uuid;                      ///< A unique identifier for the UI component.
    std::string _name;                   ///< The name of the UI component.
    std::shared_ptr<GameObject> _object; ///< The game object associated with the UI component.

  private:
    // Mapping of game objects to their associated UI components.
    static std::unordered_map<std::shared_ptr<GameObject>, std::vector<std::shared_ptr<UIComponent>>> _objectComponents;
};

} // namespace shkyera
