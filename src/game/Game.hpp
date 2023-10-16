/**
 * @file Game.hpp
 *
 * @brief Contains the declaration of the `Game` class, representing the game environment.
 */

#pragma once

#include <memory>
#include <vector>

#include "game/GameObject.hpp"

namespace shkyera {

/**
 * @brief A class representing the game environment.
 *
 * The `Game` class is responsible for managing game objects within the game environment.
 */
class Game {
  public:
    /**
     * @brief Default constructor for the `Game` class.
     */
    Game() = default;

    /**
     * @brief Default destructor for the `Game` class.
     */
    ~Game() = default;

    /**
     * @brief Get a vector of shared pointers to game objects in the game environment.
     *
     * @return A vector of shared pointers to game objects.
     */
    std::vector<std::shared_ptr<GameObject>> getGameObjects();

    /**
     * @brief Add a game object to the game environment.
     *
     * @param object A shared pointer to the game object to add.
     */
    void addGameObject(std::shared_ptr<GameObject> object);

  private:
    std::vector<std::shared_ptr<GameObject>> _gameObjects; ///< The list of game objects within the game environment.
};

} // namespace shkyera
