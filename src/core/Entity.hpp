#pragma once

#include "glm/gtc/quaternion.hpp"
#include "glm/vec3.hpp"

namespace shkyera {

class Entity {
  public:
    Entity();
    Entity(glm::vec3 position);
    Entity(glm::vec3 position, glm::vec3 orientation);

    ~Entity() = default;

    glm::vec3 &getPosition();
    glm::vec3 &getOrientation();
    glm::vec3 &getScale();

  private:
    glm::vec3 _position;
    glm::vec3 _orientation;
    glm::vec3 _scale;
};

} // namespace shkyera
