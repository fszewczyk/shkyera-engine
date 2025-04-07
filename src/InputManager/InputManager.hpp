#pragma once

#include <functional>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Common/Assert.hpp>
#include <Common/Logger.hpp>
#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <ECS/Registry.hpp>

namespace shkyera {

class InputManager {
 public:
  enum class CoordinateSystem { ABSOLUTE, SCENE };

  using Key = int;
  using MouseButton = int;

  static InputManager& getInstance();

  void setCoordinateSystem(CoordinateSystem system, glm::vec2 topLeftCorner, glm::vec2 bottomRightCorner);

  glm::vec2 getRelativeMousePosition(CoordinateSystem system);
  glm::vec2 getMousePosition(CoordinateSystem system);
  bool isMouseInside(CoordinateSystem system);

  bool isMouseButtonDown(MouseButton mouseButton) const;

  void registerKeyCallback(Key key, std::function<void()> callback);

  void unregisterKeyCallback(Key key);

  void registerMouseMoveCallback(std::function<void(double, double)> callback);

  void unregisterMouseMoveCallback();

  void registerMouseButtonDownCallback(MouseButton button, std::function<void()> callback);

  void unregisterMouseButtonDownCallback(MouseButton button);

  void registerMouseButtonUpCallback(MouseButton button, std::function<void()> callback);

  void unregisterMouseButtonUpCallback(MouseButton button);

  void processInput(GLFWwindow* window);

  template <CoordinateSystem CS, RuntimeMode RN, typename CameraTag, typename... RequiredComponents>
  static std::optional<Entity> getHoveredObject(std::shared_ptr<Registry> registry) {
    if (const auto cameraOpt = registry->getEntity<CameraTag>()) {
      const auto camera = *cameraOpt;
      const auto& mousePosition = getInstance().getRelativeMousePosition(CS);
      const auto& cameraTransform = registry->getComponent<TransformComponent>(camera);
      const auto& cameraComponent = registry->getComponent<CameraComponent>(camera);
      auto ray = cameraComponent.getRayAt(cameraTransform, mousePosition.x, 1 - mousePosition.y);

      float closestDistance = std::numeric_limits<float>::max();
      std::optional<Entity> closestEntity;
      for (const auto& [entity, boxColliderComponent] : registry->getComponentSet<BoxColliderComponent<RN>>()) {
        if (!registry->hasComponents<RequiredComponents...>(entity)) {
          continue;
        }
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, registry);
        const auto intersectionDistanceOpt = boxColliderComponent.box.intersect(ray, transformMatrix);
        if (intersectionDistanceOpt && intersectionDistanceOpt < closestDistance) {
          closestEntity = entity;
          closestDistance = *intersectionDistanceOpt;
        }
      }
      return closestEntity;
    }

    SHKYERA_ASSERT(false, "{} CameraTag not registered. Cannot find what it's hovering over", typeid(CameraTag).name());
    return std::nullopt;
  }

 private:
  InputManager() {}

  void processMouseButton(GLFWwindow* window, MouseButton button);
  void onMouseButtonDown(MouseButton button);
  void onMouseButtonUp(MouseButton button);

  std::array<bool, 10> _previousMouseButtonStates = {false};
  glm::vec2 _latestMousePosition;
  std::unordered_map<CoordinateSystem, std::pair<glm::vec2, glm::vec2>> _coordinateSystems;

  std::unordered_map<Key, std::vector<std::function<void()>>> _keyCallbacks;
  std::vector<std::function<void(double, double)>> _mouseMoveCallbacks;
  std::unordered_map<MouseButton, std::vector<std::function<void()>>> _mouseButtonDownCallbacks;
  std::unordered_map<MouseButton, std::vector<std::function<void()>>> _mouseButtonUpCallbacks;
};

}  // namespace shkyera
