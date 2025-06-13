#pragma once

#include <glm/glm.hpp>

#include <Components/TransformComponent.hpp>
#include <ECS/RegistryViewer.hpp>

namespace shkyera::utils::transform {

glm::mat4 getRotationMatrix(const TransformComponent& transform);
glm::mat4 getTransformMatrix(const TransformComponent& transform);
glm::mat4 getGlobalTransformMatrix(Entity entity, const RegistryViewer& viewer);
glm::mat4 getCameraFacingModelMatrix(const glm::mat4& baseModelMatrix, const glm::vec3& cameraPosition,
                                     const glm::mat4& viewMatrix);

}  // namespace shkyera::utils::transform
