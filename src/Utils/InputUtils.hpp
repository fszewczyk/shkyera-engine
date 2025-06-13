#pragma once

#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <ECS/RegistryViewer.hpp>
#include <InputManager/InputManager.hpp>
#include <Utils/TransformUtils.hpp>

namespace shkyera::utils::input {

template <InputManager::CoordinateSystem CS, RuntimeMode RN, typename CameraTag, typename... RequiredComponents>
std::optional<Entity> getHoveredObject(const RegistryViewer& viewer) {
  if (const auto cameraOpt = viewer.getEntity<CameraTag>()) {
    const auto camera = *cameraOpt;
    const auto& mousePosition = InputManager::getInstance().getRelativeMousePosition(CS);
    const auto& cameraTransform = viewer.read<TransformComponent>(camera);
    const auto& cameraComponent = viewer.read<CameraComponent>(camera);
    auto ray = cameraComponent.getRayAt(cameraTransform, mousePosition.x, 1 - mousePosition.y);

    float closestDistance = std::numeric_limits<float>::max();
    std::optional<Entity> closestEntity;
    for (const auto& [entity, boxColliderComponent] : viewer.readAll<BoxColliderComponent<RN>>()) {
      if (!viewer.has<RequiredComponents...>(entity)) {
        continue;
      }
      const auto& transformMatrix = shkyera::utils::transform::getGlobalTransformMatrix(entity, viewer);
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

}  // namespace shkyera::utils::input
