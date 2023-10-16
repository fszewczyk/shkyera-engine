/**
 * @file Component.hpp
 *
 * @brief Contains the declaration of the `Component` class, a base class for game components.
 */

#pragma once

#include <memory>

#include "game/GameObject.hpp"

namespace shkyera {

/**
 * @brief A base class representing a component attached to a game object.
 *
 * Components are building blocks for game objects and can be customized for various functionalities.
 */
class Component {
  public:
    /**
     * @brief Constructor to initialize a component with a shared pointer to a game object.
     *
     * @param object A shared pointer to the game object to which this component is attached.
     */
    Component(std::shared_ptr<GameObject> object);

    /**
     * @brief Get the shared pointer to the game object to which this component is attached.
     *
     * @return A shared pointer to the game object.
     */
    std::shared_ptr<GameObject> getObject();

  protected:
    std::shared_ptr<GameObject> _object; ///< The game object to which this component is attached.
};

} // namespace shkyera
