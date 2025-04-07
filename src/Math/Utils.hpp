#pragma once

#include <glm/glm.hpp>

namespace shkyera {

float toRadians(float degrees);

float toDegrees(float radians);

float angleAroundAxis(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& axis);

glm::vec3 angleAxisRotation(const glm::vec3& eulerAngles, const glm::vec3& axis, float angle);

glm::vec3 eulerAnglesFromRotationMatrix(const glm::mat3& rotationMat);

}  // namespace shkyera
