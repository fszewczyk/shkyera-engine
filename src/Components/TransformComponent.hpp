#pragma once

#include <iostream>

#include <Components/BaseComponent.hpp>

namespace shkyera {

struct Vector3 {
    float x, y, z;
};

class TransformComponent : public BaseComponent<TransformComponent> {
public:
    TransformComponent() : _position{0, 0, 0}, _rotation{0, 0, 0}, _scale{1, 1, 1} {}

    const Vector3& getPosition() const { return _position; }
    void setPosition(const Vector3& position) { _position = position; }

    const Vector3& getRotation() const { return _rotation; }
    void setRotation(const Vector3& rotation) { _rotation = rotation; }

    const Vector3& getScale() const { return _scale; }
    void setScale(const Vector3& scale) { _scale = scale; }

    void updateImpl() {
        std::cout << "Updating Transform: Position(" << _position.x << ", " << _position.y << ", " << _position.z << ")\n";
    }

private:
    Vector3 _position;
    Vector3 _rotation;
    Vector3 _scale;
};

}