#include "game/Game.hpp"

namespace shkyera {

std::vector<std::shared_ptr<GameObject>> Game::getGameObjects() { return _gameObjects; }

void Game::addGameObject(std::shared_ptr<GameObject> object) { _gameObjects.push_back(object); }

} // namespace shkyera
