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

    bool intersect(const TransformComponent& transform, const Ray& ray, float& near, float& far) const {
        glm::vec3 position = transform.getPosition();
        glm::vec3 scale = transform.getScale();
        glm::vec3 orientation = transform.getOrientation();

        glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), position);

        transformMatrix = glm::rotate(transformMatrix, orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::rotate(transformMatrix, orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        transformMatrix = glm::rotate(transformMatrix, orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        transformMatrix = glm::scale(transformMatrix, scale);

        glm::mat4 invTransformMatrix = glm::inverse(transformMatrix);
        glm::vec3 localRayOrigin = glm::vec3(invTransformMatrix * glm::vec4(ray.origin, 1.0f));
        glm::vec3 localRayDirection = glm::normalize(glm::vec3(invTransformMatrix * glm::vec4(ray.direction, 0.0f)));

        Ray localRay { localRayOrigin, localRayDirection };

        return box.intersect(localRay, near, far);
    }



    Box box;
};

} // namespace shkyera
