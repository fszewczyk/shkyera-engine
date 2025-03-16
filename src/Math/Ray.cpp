#include <Math/Ray.hpp>
#include <algorithm>

#include <iostream>

namespace shkyera {

glm::vec3 Ray::at(float t) const {
    return origin + t * direction;
}

float Ray::project(const Ray& other) const {
    glm::vec3 dir1 = glm::normalize(direction);
    glm::vec3 dir2 = glm::normalize(other.direction);

    glm::vec3 originDiff = other.origin - origin;

    float d1d1 = glm::dot(dir1, dir1);
    float d1d2 = glm::dot(dir1, dir2);
    float d1o = glm::dot(dir1, originDiff);
    float d2o = glm::dot(dir2, originDiff);

    float denominator = 1.0f - d1d2 * d1d2;

    float t1 = 0.0f, t2 = 0.0f;
    if (std::abs(denominator) > 1e-6f) {
        t1 = (d1o - d1d2 * d2o) / denominator;
    } else {
        t1 = d1o / d1d1;
    }

    return t1;
}

float Ray::project(const glm::vec3& point) const {
    // Normalize the direction vector of the ray
    glm::vec3 dirNormalized = glm::normalize(direction);

    // Vector from the ray's origin to the point
    glm::vec3 originToPoint = point - origin;

    // Compute the projection of originToPoint onto the ray's direction
    float t = glm::dot(originToPoint, dirNormalized);

    return t;
}

}  // namespace shkyera
