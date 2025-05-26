#include <Systems/ObjectSelectionSystem.hpp>

#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/CameraTags.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/SelectedEntityComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <InputManager/InputManager.hpp>
#include <Utils/InputUtils.hpp>

#include <ECS/RegistryViewer.hpp>

namespace shkyera {

ObjectSelectionSystem::ObjectSelectionSystem(std::shared_ptr<Registry> registry)
    : RegistryViewer(registry, ReadAccess<SceneCamera, CameraComponent, NameComponent>(),
                     WriteAccess<SelectedEntityComponent>()) {}

void ObjectSelectionSystem::update() {
  const auto& inputManager = InputManager::getInstance();
  if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
    if (!inputManager.isMouseInside(InputManager::CoordinateSystem::SCENE)) {
      _objectSelectedOnMouseDown = std::nullopt;
      return;
    }

    _objectSelectedOnMouseDown =
        utils::input::getHoveredObject<InputManager::CoordinateSystem::SCENE, RuntimeMode::DEVELOPMENT, SceneCamera,
                                       NameComponent>(*this);
  }

  if (inputManager.isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
    if (!_objectSelectedOnMouseDown.has_value() ||
        !InputManager::getInstance().isMouseInside(InputManager::CoordinateSystem::SCENE)) {
      _objectSelectedOnMouseDown = std::nullopt;
      return;
    }

    const auto hoveredCurrently =
        utils::input::getHoveredObject<InputManager::CoordinateSystem::SCENE, RuntimeMode::DEVELOPMENT, SceneCamera,
                                       NameComponent>(*this);
    if (hoveredCurrently.has_value() && _objectSelectedOnMouseDown.has_value() &&
        *hoveredCurrently == *_objectSelectedOnMouseDown) {
      RegistryViewer::clear<SelectedEntityComponent>();
      RegistryViewer::add<SelectedEntityComponent>(*hoveredCurrently);
      _objectSelectedOnMouseDown = std::nullopt;
    }
  }
}

}  // namespace shkyera
