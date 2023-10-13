#pragma once

#include "glm/gtc/quaternion.hpp"
#include "glm/vec3.hpp"

namespace shkyera {

/**
 * @class Entity
 * @brief A class representing an entity in a 3D world with position, orientation, and scale.
 */
class Entity {
  public:
    /**
     * @brief Default constructor for an entity with identity position, orientation, and scale.
     */
    Entity();

    /**
     * @brief Constructor for an entity with the specified position and identity orientation and scale.
     *
     * @param position The initial position of the entity.
     */
    Entity(glm::vec3 position);

    /**
     * @brief Constructor for an entity with the specified position and orientation, and identity scale.
     *
     * @param position The initial position of the entity.
     * @param orientation The initial orientation of the entity.
     */
    Entity(glm::vec3 position, glm::vec3 orientation);

    /**
     * @brief Destructor for the Entity class.
     */
    ~Entity() = default;

    /**
     * @brief Get the position of the entity.
     *
     * @return A reference to the position vector.
     */
    glm::vec3 &getPosition();

    /**
     * @brief Get the orientation of the entity.
     *
     * @return A reference to the orientation vector.
     */
    glm::vec3 &getOrientation();

    /**
     * @brief Get the scale of the entity.
     *
     * @return A reference to the scale vector.
     */
    glm::vec3 &getScale();

  private:
    glm::vec3 _position;    ///< The position of the entity.
    glm::vec3 _orientation; ///< The orientation of the entity.
    glm::vec3 _scale;       ///< The scale of the entity.
};

} // namespace shkyera
