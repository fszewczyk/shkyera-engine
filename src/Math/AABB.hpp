#pragma once

#include <glm/glm.hpp>
#include <Math/Ray.hpp>

namespace shkyera {

struct AABB {
    glm::vec3 center; 
    glm::vec3 extents;

    std::optional<float> intersect(const Ray& ray, const glm::mat4& transform) const;
};

} // namespace shkyera
