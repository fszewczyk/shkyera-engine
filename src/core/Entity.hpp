#pragma once

#include "glm/gtc/quaternion.hpp"
#include "glm/vec3.hpp"

namespace shkyera {

class Entity {
  public:
    Entity() = default;
    Entity(glm::vec3 position);
    Entity(glm::vec3 position, glm::quat orientation);

    ~Entity() = default;

  private:
    glm::vec3 _position;
    glm::quat _orientation;
};

} // namespace shkyera
