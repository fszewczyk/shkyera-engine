#pragma once

#include <Math/Ray.hpp>
#include <glm/glm.hpp>

namespace shkyera {

struct AABB {
    glm::vec3 center;
    glm::vec3 extents;

    std::optional<float> intersect(const Ray& ray, const glm::mat4& transform) const;
    bool isInside(const glm::mat4& aabbTransformMatrix, const glm::vec3& point) const;
};

}  // namespace shkyera
