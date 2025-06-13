#pragma once

#include <memory>

#include <Common/Profiler.hpp>
#include <Components/RenderingTextureComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <ECS/Registry.hpp>
#include <ECS/RegistryViewer.hpp>
#include <InputManager/InputManager.hpp>
#include <Systems/CameraMovementSystem.hpp>

#include <GLFW/glfw3.h>

namespace shkyera {

template <typename MainCameraTag>
class CameraMovementSystem : public RegistryViewer {
 public:
  CameraMovementSystem(std::shared_ptr<Registry> registry);
  void update();

 private:
  constexpr static float MOVEMENT_SPEED = 0.6;
  constexpr static float MOUSE_SENSITIVITY = 0.2;

  bool _cameraControl;
  glm::vec2 _previousMousePosition;
};

template <typename MainCameraTag>
CameraMovementSystem<MainCameraTag>::CameraMovementSystem(std::shared_ptr<Registry> registry)
    : RegistryViewer(std::move(registry), ReadAccess<MainCameraTag>(), WriteAccess<TransformComponent>()),
      _cameraControl(false) {}

template <typename MainCameraTag>
void CameraMovementSystem<MainCameraTag>::update() {
  SHKYERA_PROFILE("CameraMovementSystem::update");

  const auto& inputManager = InputManager::getInstance();

  if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
    if (inputManager.isMouseInside(InputManager::CoordinateSystem::SCENE)) {
      _cameraControl = true;
    }
  }

  if (inputManager.isMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
    _cameraControl = false;
  }

  if (inputManager.isKeyDown(GLFW_KEY_W) && _cameraControl) {
    auto& cameraTransform = RegistryViewer::write<TransformComponent>(*RegistryViewer::getEntity<MainCameraTag>());

    glm::vec3 forward;
    float yaw = cameraTransform.getOrientation().y;
    float pitch = cameraTransform.getOrientation().x;

    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(pitch);
    forward.z = sin(yaw) * cos(pitch);
    forward = glm::normalize(forward);

    cameraTransform.setPosition(cameraTransform.getPosition() + forward * MOVEMENT_SPEED);
  }

  if (inputManager.isKeyDown(GLFW_KEY_S) && _cameraControl) {
    auto& cameraTransform = RegistryViewer::write<TransformComponent>(*RegistryViewer::getEntity<MainCameraTag>());

    glm::vec3 backward;
    float yaw = cameraTransform.getOrientation().y;
    float pitch = cameraTransform.getOrientation().x;

    backward.x = -cos(yaw) * cos(pitch);
    backward.y = -sin(pitch);
    backward.z = -sin(yaw) * cos(pitch);
    backward = glm::normalize(backward);

    cameraTransform.setPosition(cameraTransform.getPosition() + backward * MOVEMENT_SPEED);
  }

  if (inputManager.isKeyDown(GLFW_KEY_A) && _cameraControl) {
    auto& cameraTransform = RegistryViewer::write<TransformComponent>(*RegistryViewer::getEntity<MainCameraTag>());

    glm::vec3 left;
    float yaw = cameraTransform.getOrientation().y;

    left.x = -sin(yaw);
    left.y = 0.0f;
    left.z = cos(yaw);
    left = glm::normalize(left);

    cameraTransform.setPosition(cameraTransform.getPosition() + left * MOVEMENT_SPEED);
  }

  if (inputManager.isKeyDown(GLFW_KEY_D) && _cameraControl) {
    auto& cameraTransform = RegistryViewer::write<TransformComponent>(*RegistryViewer::getEntity<MainCameraTag>());

    glm::vec3 right;
    float yaw = cameraTransform.getOrientation().y;

    right.x = sin(yaw);
    right.y = 0.0f;
    right.z = -cos(yaw);
    right = glm::normalize(right);

    cameraTransform.setPosition(cameraTransform.getPosition() + right * MOVEMENT_SPEED);
  }

  if (inputManager.isKeyDown(GLFW_KEY_Q) && _cameraControl) {
    auto& cameraTransform = RegistryViewer::write<TransformComponent>(*RegistryViewer::getEntity<MainCameraTag>());

    glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);  // Down direction in world space
    cameraTransform.setPosition(cameraTransform.getPosition() + down * MOVEMENT_SPEED);
  }

  if (inputManager.isKeyDown(GLFW_KEY_E) && _cameraControl) {
    auto& cameraTransform = RegistryViewer::write<TransformComponent>(*RegistryViewer::getEntity<MainCameraTag>());

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up direction in world space
    cameraTransform.setPosition(cameraTransform.getPosition() + up * MOVEMENT_SPEED);
  }

  const auto& mousePosition = inputManager.getMousePosition(InputManager::CoordinateSystem::ABSOLUTE);
  if (_cameraControl) {
    auto& cameraTransform = RegistryViewer::write<TransformComponent>(*RegistryViewer::getEntity<MainCameraTag>());

    float xOffset = static_cast<float>(mousePosition.x - _previousMousePosition.x) * MOUSE_SENSITIVITY;
    float yOffset = static_cast<float>(_previousMousePosition.y - mousePosition.y) * MOUSE_SENSITIVITY;

    glm::vec3 orientation = cameraTransform.getOrientation();
    orientation.y -= glm::radians(xOffset);
    orientation.x += glm::radians(yOffset);

    if (orientation.x > glm::radians(89.0f))
      orientation.x = glm::radians(89.0f);
    if (orientation.x < glm::radians(-89.0f))
      orientation.x = glm::radians(-89.0f);

    cameraTransform.getOrientation() = orientation;
  }
  _previousMousePosition = mousePosition;
}

}  // namespace shkyera
