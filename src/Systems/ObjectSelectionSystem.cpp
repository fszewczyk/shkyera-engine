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
        if(!InputManager::getInstance().isMouseInside(InputManager::CoordinateSystem::SCENE)) {
            _objectSelectedOnMouseDown = std::nullopt;
            return;
        }

        _objectSelectedOnMouseDown = getHoveredObject();
    });
    InputManager::getInstance().registerMouseButtonUpCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        if(!_objectSelectedOnMouseDown.has_value() && !InputManager::getInstance().isMouseInside(InputManager::CoordinateSystem::SCENE)) {
            _objectSelectedOnMouseDown = std::nullopt;
            return;
        }

        const auto hoveredCurrently = getHoveredObject();
        if(hoveredCurrently.has_value() && _objectSelectedOnMouseDown.has_value() && *hoveredCurrently == *_objectSelectedOnMouseDown)
        {
            _registry->clearSelectedEntities();
            _registry->selectEntity(*hoveredCurrently);
            _objectSelectedOnMouseDown = std::nullopt;
        }
    });
}

std::optional<Entity> ObjectSelectionSystem::getHoveredObject()
{
    const auto& camera = _registry->getCamera();
    const auto& mousePosition = InputManager::getInstance().getRelativeMousePosition(InputManager::CoordinateSystem::SCENE);
    const auto& cameraTransform = _registry->getComponent<TransformComponent>(camera);
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(camera);
    auto ray = cameraComponent.getRayAt(cameraTransform, mousePosition.x, 1 - mousePosition.y);

    float closestDistance = std::numeric_limits<float>::max();
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

    return closestEntity;
}


}
