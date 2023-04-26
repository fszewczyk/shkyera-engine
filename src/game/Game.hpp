#pragma once

#include "game/GameObject.hpp"

namespace shkyera {

class Game {
  public:
    Game() = default;
    ~Game() = default;

    /// TODO: Set method to const
    std::vector<std::shared_ptr<GameObject>> getGameObjects();

    void addGameObject(std::shared_ptr<GameObject> object);

  private:
    std::vector<std::shared_ptr<GameObject>> _gameObjects;
};

} // namespace shkyera