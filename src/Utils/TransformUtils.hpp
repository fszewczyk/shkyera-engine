#pragma once

#include <glm/glm.hpp>

namespace shkyera::utils::transform {

glm::mat4 getCameraFacingModelMatrix(const glm::mat4& baseModelMatrix, const glm::vec3& cameraPosition, const glm::mat4& viewMatrix);

}
