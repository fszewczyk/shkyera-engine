#include "core/Entity.hpp"

namespace shkyera {

Entity::Entity() : _scale(1) {}
Entity::Entity(glm::vec3 position) : _position(position), _scale(1) {}
Entity::Entity(glm::vec3 position, glm::vec3 orientation) : _position(position), _orientation(orientation), _scale(1) {}

glm::vec3 &Entity::getPosition() { return _position; }
glm::vec3 &Entity::getOrientation() { return _orientation; }
glm::vec3 &Entity::getScale() { return _scale; }

} // namespace shkyera
