#include "core/Entity.hpp"

namespace shkyera {

Entity::Entity(glm::vec3 position) : _position(position) {}
Entity::Entity(glm::vec3 position, glm::quat orientation) : _position(position), _orientation(orientation) {}

} // namespace shkyera
