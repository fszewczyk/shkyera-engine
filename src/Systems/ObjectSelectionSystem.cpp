#include <Systems/ObjectSelectionSystem.hpp>

#include <Components/BoxColliderComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <InputManager/InputManager.hpp>

#include <iostream>

namespace shkyera {

ObjectSelectionSystem::ObjectSelectionSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    InputManager::getInstance().registerMouseButtonDownCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        tryToSelectObject();
    });
}

void ObjectSelectionSystem::setOnSelectCallback(std::function<void(Entity)> callback)
{
    _onSelectCallback = callback;
}

void ObjectSelectionSystem::tryToSelectObject() {
    const auto& camera = _registry->getCamera();
    const auto& mousePosition = InputManager::getInstance().getRelativeMousePosition(InputManager::CoordinateSystem::SCENE);

    if(mousePosition.x < 0 || mousePosition.y < 0 || mousePosition.x >= 1.0 || mousePosition.y >= 1.0) {
        return;
    }

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(camera);
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(camera);
    auto ray = cameraComponent.getRayAt(cameraTransform, mousePosition.x, 1 - mousePosition.y);

    float closestDistance = FLT_MAX;
    std::optional<Entity> closestEntity;
    for(const auto& [entity, boxColliderComponent] : _registry->getComponentSet<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>()) {
        const auto& transform = _registry->getComponent<TransformComponent>(entity);
        float near, far;
        bool intersected = boxColliderComponent.intersect(transform, ray, near, far);
        if(intersected && near < closestDistance)
        {
            closestEntity = entity;
            closestDistance = near;
        }
    }

    if(closestEntity && _onSelectCallback)
    {
        _onSelectCallback(*closestEntity);
    }
}

}
