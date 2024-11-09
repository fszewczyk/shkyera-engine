#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Math/Utils.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class TransformComponent : public BaseComponent<TransformComponent> {
public:
    TransformComponent()
        : _position(0.0f, 0.0f, 0.0f),
          _orientation(0.0, 0.0, 0.0),
          _scale(1.0f, 1.0f, 1.0f) {}

    glm::vec3& getPosition() { return _position; }
    const glm::vec3& getPosition() const { return _position; }
    void setPosition(const glm::vec3& position) { _position = position; }

    glm::vec3& getOrientation() { return _orientation; }
    const glm::vec3& getOrientation() const { return _orientation; }
    void setOrientation(const glm::vec3& orientation) { _orientation = orientation; }

    glm::vec3& getScale() { return _scale; }
    const glm::vec3& getScale() const { return _scale; }
    void setScale(const glm::vec3& scale) { _scale = scale; }

    glm::mat4 getRotationMatrix() const {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), _orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, _orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, _orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        return rotationMatrix;
    }

    glm::mat4 getTransformMatrix() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
        glm::mat4 rotationMatrix = getRotationMatrix();
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

private:
    glm::vec3 _position;
    glm::vec3 _orientation;
    glm::vec3 _scale;
};

} // namespace shkyera
