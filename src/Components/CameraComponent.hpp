#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/TransformComponent.hpp>

namespace shkyera {

class CameraComponent {
public:
    enum class ProjectionType {
        Perspective,
        Orthographic
    };

    CameraComponent(float fov = 45.0f, float aspectRatio = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 10000.0f, ProjectionType projectionType = ProjectionType::Perspective)
        : fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), projectionType(projectionType) {}

    float fov;
    float aspectRatio;
    float nearPlane, farPlane;
    ProjectionType projectionType;
    
    glm::mat4 getViewMatrix(const TransformComponent& transformComponent) const {
        glm::vec3 position = transformComponent.getPosition();
        glm::vec3 orientation = transformComponent.getOrientation();
        
        // Calculate direction vectors
        glm::vec3 front;
        front.x = cos(orientation.y) * cos(orientation.x);
        front.y = sin(orientation.x);
        front.z = sin(orientation.y) * cos(orientation.x);
        front = glm::normalize(front);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(front, up));
        up = -glm::normalize(glm::cross(right, front));

        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix() const {
        if (projectionType == ProjectionType::Perspective) {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        } else {
            float orthoSize = 10.0f;  // Can be configurable
            float halfWidth = orthoSize * aspectRatio * 0.5f;
            float halfHeight = orthoSize * 0.5f;
            return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
        }
    }
};

} // namespace shkyera
