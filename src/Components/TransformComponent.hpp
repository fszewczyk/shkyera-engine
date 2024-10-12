#pragma once

#include <iostream>
#include <Components/BaseComponent.hpp>

namespace shkyera {

/**
 * Represents a 3D vector.
 * Contains three floating-point values representing 
 * the x, y, and z coordinates.
 */
struct Vector3 {
    float x; /**< X coordinate */
    float y; /**< Y coordinate */
    float z; /**< Z coordinate */
};

/**
 * Component for managing 3D transformations.
 * Inherits from BaseComponent to handle position, rotation, 
 * and scale in a 3D space.
 */
class TransformComponent : public BaseComponent<TransformComponent> {
public:
    /**
     * Default constructor.
     * Initializes position to (0, 0, 0), rotation to (0, 0, 0), 
     * and scale to (1, 1, 1).
     */
    TransformComponent() : _position{0, 0, 0}, _rotation{0, 0, 0}, _scale{1, 1, 1} {}

    /**
     * Gets the current position.
     * 
     * @return Reference to the current position vector.
     */
    const Vector3& getPosition() const { return _position; }

    /**
     * Sets the position.
     * Updates the position vector to the specified value.
     * 
     * @param position New position vector.
     */
    void setPosition(const Vector3& position) { _position = position; }

    /**
     * Gets the current rotation.
     * 
     * @return Reference to the current rotation vector.
     */
    const Vector3& getRotation() const { return _rotation; }

    /**
     * Sets the rotation.
     * Updates the rotation vector to the specified value.
     * 
     * @param rotation New rotation vector.
     */
    void setRotation(const Vector3& rotation) { _rotation = rotation; }

    /**
     * Gets the current scale.
     * 
     * @return Reference to the current scale vector.
     */
    const Vector3& getScale() const { return _scale; }

    /**
     * Sets the scale.
     * Updates the scale vector to the specified value.
     * 
     * @param scale New scale vector.
     */
    void setScale(const Vector3& scale) { _scale = scale; }

    /**
     * Implements update logic.
     * Currently, no functionality is defined for the update logic.
     */
    void updateImpl() {
    }

private:
    Vector3 _position; /**< Current position in 3D space. */
    Vector3 _rotation; /**< Current rotation in 3D space. */
    Vector3 _scale;    /**< Current scale in 3D space. */
};

} // namespace shkyera
