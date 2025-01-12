#pragma once

#include <string>

#include <Math/Box.hpp>
#include <Components/BaseComponent.hpp>
#include <Components/TransformComponent.hpp>

namespace shkyera {

template<RuntimeMode Mode = RuntimeMode::PRODUCTION>
class BoxColliderComponent : public BaseComponent<BoxColliderComponent<Mode>, Mode> {
public:
    BoxColliderComponent() = default;
    BoxColliderComponent(const Box& box) : box(box) {}

    bool intersect(const glm::mat4& transformMatrix, const Ray& ray, float& near, float& far) const {
        glm::mat4 invTransformMatrix = glm::inverse(transformMatrix);
        glm::vec3 localRayOrigin = glm::vec3(invTransformMatrix * glm::vec4(ray.origin, 1.0f));
        glm::vec3 localRayDirection = glm::normalize(glm::vec3(invTransformMatrix * glm::vec4(ray.direction, 0.0f)));

        Ray localRay { localRayOrigin, localRayDirection };

        return box.intersect(localRay, near, far);
    }



    Box box;
};

} // namespace shkyera
