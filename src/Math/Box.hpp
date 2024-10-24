#pragma once

#include <glm/glm.hpp>
#include <Math/Ray.hpp>

namespace shkyera {

struct Box {
    glm::vec3 center; 
    glm::vec3 extents;
    glm::mat3 orientation;

    Box() = default;
    Box(const glm::vec3& minBounds, const glm::vec3& maxBounds);
    Box(const glm::vec3& center, const glm::vec3& extents, const glm::mat3& orientation);

    bool intersect(const Ray& ray, float& tNear, float& tFar) const;
};

} // namespace shkyera
