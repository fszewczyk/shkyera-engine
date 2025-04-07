#pragma once

#include <glm/glm.hpp>

namespace shkyera {

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;

  /**
     * Get the coordinate of the point on the ray at time t.
     * @return The point along the ray that is t units away from origin.
     */
  glm::vec3 at(float t) const;

  /**
     * Project another ray onto this ray. Find the point on this ray that is closest to the other ray.
     * Project other ray onto this ray and find the distance from this ray's origin
     * to the projection as a unit of normalized direction
     * @param other Ray which should be projected
     * @return distance from origin in the ray's direction to the closest point (a non-negative value)
     */
  float project(const Ray& other) const;

  /**
     * Project another point onto this ray. Find the point on this ray that is closest to the other ray.
     * Project other ray onto this ray and find the distance from this ray's origin
     * to the projection as a unit of normalized direction.
     * @param other Ray which should be projected
     * @return distance from origin in the ray's direction to the closest point (a non-negative value)
     */
  float project(const glm::vec3& point) const;
};

}  // namespace shkyera
