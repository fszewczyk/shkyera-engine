/**
 * @file Runtime.hpp
 *
 * @brief Contains the declaration of the `Runtime` class, representing the game environment.
 */

#pragma once

#include <memory>
#include <vector>

#include <ECS/Registry.hpp>

namespace shkyera {

/**
 * @brief A class representing the game environment.
 *
 * The `Runtime` class is responsible for managing game objects within the game environment.
 */
class Runtime {
 public:
  /**
     * @brief Default constructor for the `Runtime` class.
     */
  Runtime(std::shared_ptr<Registry> registry);

  void run();

  /**
     * @brief Default destructor for the `Runtime` class.
     */
  ~Runtime() = default;

 private:
  std::shared_ptr<Registry> _registry;
};

}  // namespace shkyera
