#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <Math/Utils.hpp>
#include <cmath>

namespace shkyera {

float toRadians(float degrees) {
  return degrees * M_PI / 180;
}

float toDegrees(float radians) {
  return radians * 180 / M_PI;
}

float angleAroundAxis(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& axis) {
  float dotProduct = glm::dot(v1, v2);
  float magnitudes = glm::length(v1) * glm::length(v2);

  if (magnitudes < 1e-6f) {
    return 0.0f;
  }

  float cosTheta = glm::clamp(dotProduct / magnitudes, -1.0f, 1.0f);

  float angle = std::acos(cosTheta);

  glm::vec3 crossProduct = glm::cross(v1, v2);
  float sign = glm::dot(crossProduct, axis);

  if (sign < 0.0f) {
    angle = -angle;
  }

  return angle;
}

glm::vec3 angleAxisRotation(const glm::vec3& eulerAngles, const glm::vec3& axis, float angle) {
  glm::quat originalRotation = glm::quat(eulerAngles);
  glm::quat angleAxisRotation = glm::angleAxis(angle, glm::normalize(axis));
  glm::quat newRotation = angleAxisRotation * originalRotation;

  return glm::eulerAngles(newRotation);
}

glm::vec3 eulerAnglesFromRotationMatrix(const glm::mat3& rotationMat) {
  glm::vec3 eulerAngles;

  const float m00 = rotationMat[0][0];
  const float m01 = rotationMat[0][1];
  const float m02 = rotationMat[0][2];
  const float m10 = rotationMat[1][0];
  const float m11 = rotationMat[1][1];
  const float m12 = rotationMat[1][2];
  const float m20 = rotationMat[2][0];
  const float m21 = rotationMat[2][1];
  const float m22 = rotationMat[2][2];

  // Calculate pitch (x-axis rotation)
  if (std::abs(m20) < 1.0f) {
    // Standard case
    eulerAngles.y = std::asin(-m20);       // pitch
    eulerAngles.z = std::atan2(m10, m00);  // yaw
    eulerAngles.x = std::atan2(m21, m22);  // roll
  } else {
    // Gimbal lock case
    eulerAngles.y = m20 > 0 ? -glm::half_pi<float>() : glm::half_pi<float>();
    eulerAngles.z = std::atan2(-m01, m11);
    eulerAngles.x = 0.0f;  // Roll is indeterminate
  }

  return -eulerAngles;
}

}  // namespace shkyera
