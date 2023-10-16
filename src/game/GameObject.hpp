/**
 * @file GameObject.hpp
 *
 * @brief Contains the declaration of the `GameObject` class, representing a game object in the system.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/Entity.hpp"

namespace shkyera {

/**
 * @brief A class representing a game object.
 *
 * The `GameObject` class is a fundamental entity within the game system and inherits from the `Entity` class.
 * It encapsulates data related to game objects, such as their names.
 */
class GameObject : public Entity {
  public:
    /**
     * @brief Constructor to create a game object with a specified name.
     *
     * @param name The name of the game object.
     */
    GameObject(std::string name);

    /**
     * @brief Get the name of the game object.
     *
     * @return A string containing the name of the game object.
     */
    std::string getName() const;

  private:
    std::string _name; ///< The name of the game object.
};

} // namespace shkyera
