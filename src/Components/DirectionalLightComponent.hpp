#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>

#include <Components/BaseComponent.hpp>
#include <Components/TransformComponent.hpp>

namespace shkyera {

class DirectionalLightComponent : public BaseComponent<DirectionalLightComponent> {
public:
    float intensity = 0.5;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    glm::mat4 getViewMatrix(const TransformComponent& lightTransformComponent, const TransformComponent& cameraTransformComponent) const {
        glm::vec3 orientation = lightTransformComponent.getOrientation();
        
        glm::vec3 front;
        front.x = std::cos(orientation.y) * std::cos(orientation.x);
        front.y = std::sin(orientation.x);
        front.z = std::sin(orientation.y) * std::cos(orientation.x);
        front = glm::normalize(front);

        glm::vec3 position = cameraTransformComponent.getPosition() - 100.0f * front;

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(front, up));
        up = -glm::normalize(glm::cross(right, front));

        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix(uint8_t levelOfDetail) const {
        constexpr float NearPlane = 0.1f;
        constexpr float FarPlane = 1000.0f;
        constexpr float DefaultHalfSize = 10.0f;
        constexpr float levelOfDetailExpansion = 2.0f;

        float halfSize = powf(levelOfDetailExpansion, levelOfDetail) * DefaultHalfSize;

        return glm::ortho(-halfSize, halfSize, -halfSize, halfSize, NearPlane, FarPlane);
    }

    static glm::vec3 getDirection(const TransformComponent& lightTransformComponent)
    {
        const auto& orientation = lightTransformComponent.getOrientation();
        glm::vec3 front;
        front.x = std::cos(orientation.y) * std::cos(orientation.x);
        front.y = std::sin(orientation.x);
        front.z = std::sin(orientation.y) * std::cos(orientation.x);
        front = glm::normalize(front);
        return -front;
    }
};

} // namespace shkyera
