#include "game/GameObject.hpp"

namespace shkyera {

GameObject::GameObject(std::string name) : _name(name) {}

void GameObject::addComponent(std::shared_ptr<Component> component) { _components.push_back(component); }

std::string GameObject::getName() const { return _name; }

} // namespace shkyera