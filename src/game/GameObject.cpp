#include "game/GameObject.hpp"

namespace shkyera {

GameObject::GameObject(std::string name) : _name(name) {}

std::string GameObject::getName() const { return _name; }

} // namespace shkyera