#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>

#include <Components/BaseComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/CameraComponent.hpp>

namespace shkyera {

class DirectionalLightComponent : public BaseComponent<DirectionalLightComponent> {
public:
    float intensity = 0.5;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};

    inline static uint8_t LevelsOfDetail = 4;
    inline static std::vector<float> CascadePlanes = {0.01, 2.0, 5.0, 10.0, 100.0};

    glm::mat4 getLightSpaceMatrix(
        const TransformComponent& lightTransformComponent,
        const TransformComponent& cameraTransformComponent,
        const CameraComponent& cameraComponent,
        uint8_t levelOfDetail) const
    {
        auto frustumCorners = cameraComponent.getFrustumCornersWorldSpace(CascadePlanes[levelOfDetail], CascadePlanes[levelOfDetail + 1], cameraTransformComponent);

        glm::vec3 front = getDirection(lightTransformComponent);
        glm::vec3 center = glm::vec3(0.0f);

        for (const auto& corner : frustumCorners)
            center += corner;
        center /= frustumCorners.size();

        glm::mat4 lightView = glm::lookAt(center + front * 50.0f, center, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());

        for (const auto& corner : frustumCorners) {
            glm::vec3 cornerInLightSpace = glm::vec3(lightView * glm::vec4(corner, 1.0f));
            min = glm::min(min, cornerInLightSpace);
            max = glm::max(max, cornerInLightSpace);
        }

        glm::mat4 lightProjection = glm::ortho(min.x, max.x, min.y, max.y, 0.1f, 100.0f);
        return lightProjection * lightView;
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
