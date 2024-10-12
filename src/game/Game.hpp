/**
 * @file Game.hpp
 *
 * @brief Contains the declaration of the `Game` class, representing the game environment.
 */

#pragma once

#include <memory>
#include <vector>

#include <ECS/Registry.hpp>

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

    void run();

    /**
     * @brief Default destructor for the `Game` class.
     */
    ~Game() = default;

  private:
    Registry _registry;
};

} // namespace shkyera
