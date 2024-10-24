#include <Math/Box.hpp>
#include <algorithm>

namespace shkyera {

Box::Box(const glm::vec3& minBounds, const glm::vec3& maxBounds) {
    center = (minBounds + maxBounds) * 0.5f;
    extents = (maxBounds - minBounds) * 0.5f;
    orientation = glm::mat3(1.0f);
}

Box::Box(const glm::vec3& center, const glm::vec3& extents, const glm::mat3& orientation)
    : center(center), extents(extents), orientation(orientation) {}

bool Box::intersect(const Ray& ray, float& tNear, float& tFar) const {
    glm::vec3 localRayDir = orientation * ray.direction;
    glm::vec3 localRayOrig = orientation * (ray.origin - center);

    tNear = -FLT_MAX;
    tFar = FLT_MAX;

    for (int i = 0; i < 3; ++i) {
        if (glm::abs(localRayDir[i]) < 1e-6f) {
            if (localRayOrig[i] < -extents[i] || localRayOrig[i] > extents[i]) {
                return false;
            }
        } else {
            float t1 = (-extents[i] - localRayOrig[i]) / localRayDir[i];
            float t2 = (extents[i] - localRayOrig[i]) / localRayDir[i];

            if (t1 > t2) std::swap(t1, t2);

            tNear = std::max(tNear, t1);
            tFar = std::min(tFar, t2);

            if (tNear > tFar) {
                return false;
            }
        }
    }

    return tNear >= 0.0f;
}

} // namespace shkyera
