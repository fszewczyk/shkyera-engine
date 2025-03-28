#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Math/Utils.hpp>
#include <ECS/Registry.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class TransformComponent : public BaseComponent<TransformComponent> {
public:
    TransformComponent() = default;

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

    static glm::mat4 getGlobalRotationMatrix(Entity entity, std::shared_ptr<Registry> registry) {
        std::vector<glm::mat4> transforms;
        auto localTransform = registry->getComponent<TransformComponent>(entity).getRotationMatrix();
        transforms.emplace_back(std::move(localTransform));

        const auto& hierarchy = registry->getHierarchy();
        while(const auto& parentOpt = hierarchy.getParent(entity))
        {
            entity = *parentOpt;
            if(registry->hasComponent<TransformComponent>(entity))
            {
                auto localParentTransform = registry->getComponent<TransformComponent>(entity).getRotationMatrix();
                transforms.emplace_back(std::move(localParentTransform));
            }
        }

        auto result = glm::mat4(1.0);
        for(auto it = transforms.rbegin(); it != transforms.rend(); ++it)
        {
            result = result * (*it);
        }
        return result;
    }

    static glm::mat4 getGlobalTransformMatrix(Entity entity, std::shared_ptr<Registry> registry)
    {
        std::vector<glm::mat4> transforms;
        auto localTransform = registry->getComponent<TransformComponent>(entity).getTransformMatrix();
        transforms.emplace_back(std::move(localTransform));

        const auto& hierarchy = registry->getHierarchy();
        while(const auto& parentOpt = hierarchy.getParent(entity))
        {
            entity = *parentOpt;
            if(registry->hasComponent<TransformComponent>(entity))
            {
                auto localParentTransform = registry->getComponent<TransformComponent>(entity).getTransformMatrix();
                transforms.emplace_back(std::move(localParentTransform));
            }
        }

        auto result = glm::mat4(1.0);
        for(auto it = transforms.rbegin(); it != transforms.rend(); ++it)
        {
            result = result * (*it);
        }
        return result;
    }

private:
    glm::vec3 _position {0.0, 0.0, 0.0};
    glm::vec3 _orientation {0.0, 0.0, 0.0};
    glm::vec3 _scale {1.0, 1.0, 1.0};
};

} // namespace shkyera
