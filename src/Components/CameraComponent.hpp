#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/TransformComponent.hpp>

namespace shkyera {

class CameraComponent : public TransformComponent {
public:
    enum class ProjectionType {
        Perspective,
        Orthographic
    };

    CameraComponent(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 10000.0f, ProjectionType projectionType = ProjectionType::Perspective)
        : _fov(fov), _aspectRatio(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane), _projectionType(projectionType) {}

    // Set and get Field of View (for perspective projection)
    float& getFov() { return _fov; }                             // Modifiable reference
    const float& getFov() const { return _fov; }                 // Constant reference

    // Set and get aspect ratio (width/height of the screen)
    float& getAspectRatio() { return _aspectRatio; }             // Modifiable reference
    const float& getAspectRatio() const { return _aspectRatio; } // Constant reference

    // Set and get near/far clipping planes
    float& getNearPlane() { return _nearPlane; }                 // Modifiable reference
    const float& getNearPlane() const { return _nearPlane; }     // Constant reference

    float& getFarPlane() { return _farPlane; }                   // Modifiable reference
    const float& getFarPlane() const { return _farPlane; }       // Constant reference

    // Set projection type (Perspective or Orthographic)
    ProjectionType& getProjectionType() { return _projectionType; }  // Modifiable reference
    const ProjectionType& getProjectionType() const { return _projectionType; } // Constant reference

    // Get the view matrix based on the camera's transform
    glm::mat4 getViewMatrix() const {
        glm::vec3 position = getPosition();
        glm::vec3 orientation = getOrientation();
        
        // Calculate direction vectors
        glm::vec3 front;
        front.x = cos(orientation.y) * cos(orientation.x);
        front.y = sin(orientation.x);
        front.z = sin(orientation.y) * cos(orientation.x);
        front = glm::normalize(front);

        // Define the world up vector (Y-axis)
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        // Get the camera's right vector using the front and world up
        glm::vec3 right = glm::normalize(glm::cross(front, up));

        // Recalculate up vector based on the camera's orientation
        up = -glm::normalize(glm::cross(right, front));

        return glm::lookAt(position, position + front, up);
    }

    // Get the projection matrix (Perspective or Orthographic)
    glm::mat4 getProjectionMatrix() const {
        if (_projectionType == ProjectionType::Perspective) {
            return glm::perspective(glm::radians(_fov), _aspectRatio, _nearPlane, _farPlane);
        } else {
            float orthoSize = 10.0f;  // Can be configurable
            float halfWidth = orthoSize * _aspectRatio * 0.5f;
            float halfHeight = orthoSize * 0.5f;
            return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, _nearPlane, _farPlane);
        }
    }

private:
    float _fov;                  // Field of View (for perspective projection)
    float _aspectRatio;           // Aspect ratio (width/height)
    float _nearPlane, _farPlane;  // Near and far clipping planes
    ProjectionType _projectionType;  // Type of projection (Perspective/Orthographic)
};

} // namespace shkyera
