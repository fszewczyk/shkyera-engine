#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <Math/AABB.hpp>
#include <Common/Logger.hpp>
#include <algorithm>

namespace shkyera {

std::optional<float> AABB::intersect(const Ray& ray, const glm::mat4& transform) const {
    glm::vec3 minBounds = center - extents;
    glm::vec3 maxBounds = center + extents;

    glm::vec3 transformedMin = glm::vec3(transform * glm::vec4(minBounds, 1.0f));
    glm::vec3 transformedMax = glm::vec3(transform * glm::vec4(maxBounds, 1.0f));

    glm::vec3 worldMin = glm::min(transformedMin, transformedMax);
    glm::vec3 worldMax = glm::max(transformedMin, transformedMax);

    float tMin = 0.0f;
    float tMax = std::numeric_limits<float>::max();

    for (int i = 0; i < 3; ++i) {
        if (std::abs(ray.direction[i]) < std::numeric_limits<float>::epsilon()) 
        {
            if (ray.origin[i] < worldMin[i] || ray.origin[i] > worldMax[i]) 
            {
                return std::nullopt;
            }
        } 
        else
        {
            float t1 = (worldMin[i] - ray.origin[i]) / ray.direction[i];
            float t2 = (worldMax[i] - ray.origin[i]) / ray.direction[i];

            if (t1 > t2) 
            {
                std::swap(t1, t2);
            }

            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            if (tMin > tMax) 
            {
                return std::nullopt;
            }
        }
    }

    return tMin;
}

} // namespace shkyera