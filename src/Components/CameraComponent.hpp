#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Math/Ray.hpp>
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
        
        glm::vec3 front;
        front.x = std::cos(orientation.y) * std::cos(orientation.x);
        front.y = std::sin(orientation.x);
        front.z = std::sin(orientation.y) * std::cos(orientation.x);
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

    Ray getRayAt(const TransformComponent& transformComponent, float x, float y) const {
        glm::mat4 viewMatrix = getViewMatrix(transformComponent);
        
        glm::mat4 invViewProj = glm::inverse(getProjectionMatrix() * viewMatrix);
        
        float ndcX = 2.0f * x - 1.0f;
        float ndcY = 1.0f - 2.0f * y;

        glm::vec4 nearPoint = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
        glm::vec4 farPoint = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

        glm::vec4 nearWorld = invViewProj * nearPoint;
        glm::vec4 farWorld = invViewProj * farPoint;

        nearWorld /= nearWorld.w;
        farWorld /= farWorld.w;

        glm::vec3 rayOrigin = glm::vec3(nearWorld);
        
        glm::vec3 rayDirection = glm::normalize(glm::vec3(farWorld) - rayOrigin);

        return { rayOrigin, rayDirection };
    }

};

} // namespace shkyera
