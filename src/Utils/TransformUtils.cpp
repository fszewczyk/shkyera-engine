#include <Utils/TransformUtils.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace shkyera::utils::transform {

glm::mat4 getRotationMatrix(const TransformComponent& transform) {
  const auto& orientation = transform.getOrientation();
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  rotationMatrix = glm::rotate(rotationMatrix, orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  rotationMatrix = glm::rotate(rotationMatrix, orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  return rotationMatrix;
}

glm::mat4 getTransformMatrix(const TransformComponent& transform) {
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.getPosition());
  glm::mat4 rotationMatrix = getRotationMatrix(transform);
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.getScale());

  return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 getGlobalTransformMatrix(Entity entity, const RegistryViewer& viewer) {
  std::vector<glm::mat4> transforms;
  auto localTransform = getTransformMatrix(viewer.read<TransformComponent>(entity));
  transforms.emplace_back(std::move(localTransform));

  while (const auto& parentOpt = viewer.getParent(entity)) {
    entity = *parentOpt;
    if (viewer.has<TransformComponent>(entity)) {
      auto localParentTransform = viewer.read<TransformComponent>(entity).getTransformMatrix();
      transforms.emplace_back(std::move(localParentTransform));
    }
  }

  auto result = glm::mat4(1.0);
  for (auto it = transforms.rbegin(); it != transforms.rend(); ++it) {
    result = result * (*it);
  }

  return result;
}

glm::mat4 getCameraFacingModelMatrix(const glm::mat4& baseMatrix, const glm::vec3& cameraPosition,
                                     const glm::mat4& viewMatrix) {
  glm::vec3 position = glm::vec3(baseMatrix[3]);

  glm::vec3 scaleVec(glm::length(glm::vec3(baseMatrix[0])), glm::length(glm::vec3(baseMatrix[1])),
                     glm::length(glm::vec3(baseMatrix[2])));

  glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

  glm::mat4 rotation(1.0f);
  glm::mat3 camRot = glm::mat3(viewMatrix);
  camRot = glm::transpose(camRot);
  rotation = glm::mat4(camRot);
  rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(0, 0, 1));

  glm::mat4 correction = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scaleVec);

  glm::mat4 model = translation * rotation * correction * scaling;
  return model;
}

}  // namespace shkyera::utils::transform
