#include <Systems/ObjectSelectionSystem.hpp>

#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <InputManager/InputManager.hpp>

#include <iostream>

namespace shkyera {

ObjectSelectionSystem::ObjectSelectionSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    InputManager::getInstance().registerMouseButtonDownCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        if (!InputManager::getInstance().isMouseInside(InputManager::CoordinateSystem::SCENE)) {
            _objectSelectedOnMouseDown = std::nullopt;
            return;
        }

        _objectSelectedOnMouseDown = InputManager::getHoveredObject<
            InputManager::CoordinateSystem::SCENE,
            RuntimeMode::DEVELOPMENT,
            NameComponent>(_registry);
    });
    InputManager::getInstance().registerMouseButtonUpCallback(GLFW_MOUSE_BUTTON_LEFT, [this]() {
        if (!_objectSelectedOnMouseDown.has_value() || !InputManager::getInstance().isMouseInside(InputManager::CoordinateSystem::SCENE)) {
            _objectSelectedOnMouseDown = std::nullopt;
            return;
        }

        const auto hoveredCurrently = InputManager::getHoveredObject<
            InputManager::CoordinateSystem::SCENE,
            RuntimeMode::DEVELOPMENT,
            NameComponent>(_registry);
        if (hoveredCurrently.has_value() && _objectSelectedOnMouseDown.has_value() && *hoveredCurrently == *_objectSelectedOnMouseDown) {
            _registry->clearSelectedEntities();
            _registry->selectEntity(*hoveredCurrently);
            _objectSelectedOnMouseDown = std::nullopt;
        }
    });
}

}  // namespace shkyera
