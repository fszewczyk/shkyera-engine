#pragma once

#include <memory>

#include <Components/CameraTags.hpp>
#include <Components/TransformComponent.hpp>
#include <ECS/Registry.hpp>
#include <InputManager/InputManager.hpp>
#include <Systems/CameraMovementSystem.hpp>
#include <iostream>

namespace shkyera {

template <typename MainCameraTag>
class CameraMovementSystem {
 public:
  CameraMovementSystem(std::shared_ptr<Registry> registry);
  void update();

 private:
  constexpr static float MOVEMENT_SPEED = 0.6;
  constexpr static float MOUSE_SENSITIVITY = 0.2;

  void setupCameraMovement();

  bool _cameraControl;
  std::shared_ptr<Registry> _registry;
};

template <typename MainCameraTag>
CameraMovementSystem<MainCameraTag>::CameraMovementSystem(std::shared_ptr<Registry> registry)
    : _registry(registry), _cameraControl(false) {
  setupCameraMovement();
}

template <typename MainCameraTag>
void CameraMovementSystem<MainCameraTag>::update() {}

template <typename MainCameraTag>
void CameraMovementSystem<MainCameraTag>::setupCameraMovement() {
  SHKYERA_ASSERT(_registry->getEntity<MainCameraTag>().has_value(),
                 "There is no entity with component {}. Cannot initialize CameraMovementSystem",
                 typeid(MainCameraTag).name());

  auto& inputManager = InputManager::getInstance();

  inputManager.registerMouseButtonDownCallback(GLFW_MOUSE_BUTTON_RIGHT, [this, &inputManager]() {
    if (inputManager.isMouseInside(InputManager::CoordinateSystem::SCENE)) {
      _cameraControl = true;
    }
  });

  inputManager.registerMouseButtonUpCallback(GLFW_MOUSE_BUTTON_RIGHT, [this]() { _cameraControl = false; });

  inputManager.registerKeyCallback(GLFW_KEY_W, [this]() {
    auto& cameraTransform = _registry->getComponent<TransformComponent>(*_registry->getEntity<MainCameraTag>());

    if (_cameraControl) {
      glm::vec3 forward;
      float yaw = cameraTransform.getOrientation().y;
      float pitch = cameraTransform.getOrientation().x;

      forward.x = cos(yaw) * cos(pitch);
      forward.y = sin(pitch);
      forward.z = sin(yaw) * cos(pitch);
      forward = glm::normalize(forward);

      cameraTransform.setPosition(cameraTransform.getPosition() + forward * MOVEMENT_SPEED);
    }
  });

  inputManager.registerKeyCallback(GLFW_KEY_S, [this]() {
    auto& cameraTransform = _registry->getComponent<TransformComponent>(*_registry->getEntity<MainCameraTag>());

    if (_cameraControl) {
      glm::vec3 backward;
      float yaw = cameraTransform.getOrientation().y;
      float pitch = cameraTransform.getOrientation().x;

      backward.x = -cos(yaw) * cos(pitch);
      backward.y = -sin(pitch);
      backward.z = -sin(yaw) * cos(pitch);
      backward = glm::normalize(backward);

      cameraTransform.setPosition(cameraTransform.getPosition() + backward * MOVEMENT_SPEED);
    }
  });

  inputManager.registerKeyCallback(GLFW_KEY_A, [this]() {
    auto& cameraTransform = _registry->getComponent<TransformComponent>(*_registry->getEntity<MainCameraTag>());

    if (_cameraControl) {
      glm::vec3 left;
      float yaw = cameraTransform.getOrientation().y;

      left.x = -sin(yaw);
      left.y = 0.0f;
      left.z = cos(yaw);
      left = glm::normalize(left);

      cameraTransform.setPosition(cameraTransform.getPosition() + left * MOVEMENT_SPEED);
    }
  });

  inputManager.registerKeyCallback(GLFW_KEY_D, [this]() {
    auto& cameraTransform = _registry->getComponent<TransformComponent>(*_registry->getEntity<MainCameraTag>());

    if (_cameraControl) {
      glm::vec3 right;
      float yaw = cameraTransform.getOrientation().y;

      right.x = sin(yaw);
      right.y = 0.0f;
      right.z = -cos(yaw);
      right = glm::normalize(right);

      cameraTransform.setPosition(cameraTransform.getPosition() + right * MOVEMENT_SPEED);
    }
  });

  inputManager.registerKeyCallback(GLFW_KEY_Q, [this]() {
    auto& cameraTransform = _registry->getComponent<TransformComponent>(*_registry->getEntity<MainCameraTag>());

    if (_cameraControl) {
      glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);  // Down direction in world space
      cameraTransform.setPosition(cameraTransform.getPosition() + down * MOVEMENT_SPEED);
    }
  });

  inputManager.registerKeyCallback(GLFW_KEY_E, [this]() {
    auto& cameraTransform = _registry->getComponent<TransformComponent>(*_registry->getEntity<MainCameraTag>());

    if (_cameraControl) {
      glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up direction in world space
      cameraTransform.setPosition(cameraTransform.getPosition() + up * MOVEMENT_SPEED);
    }
  });

  inputManager.registerMouseMoveCallback([this](double xPos, double yPos) {
    auto& cameraTransform = _registry->getComponent<TransformComponent>(*_registry->getEntity<MainCameraTag>());

    static double lastX = xPos, lastY = yPos;

    float xOffset = static_cast<float>(xPos - lastX) * MOUSE_SENSITIVITY;
    float yOffset = static_cast<float>(lastY - yPos) * MOUSE_SENSITIVITY;

    lastX = xPos;
    lastY = yPos;

    if (_cameraControl) {
      glm::vec3 orientation = cameraTransform.getOrientation();
      orientation.y -= glm::radians(xOffset);
      orientation.x += glm::radians(yOffset);

      if (orientation.x > glm::radians(89.0f))
        orientation.x = glm::radians(89.0f);
      if (orientation.x < glm::radians(-89.0f))
        orientation.x = glm::radians(-89.0f);

      cameraTransform.getOrientation() = orientation;
    }
  });
}

}  // namespace shkyera
