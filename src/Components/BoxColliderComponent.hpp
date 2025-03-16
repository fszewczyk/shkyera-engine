#pragma once

#include <string>

#include <Components/BaseComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Math/AABB.hpp>

namespace shkyera {

template <RuntimeMode Mode = RuntimeMode::PRODUCTION>
class BoxColliderComponent : public BaseComponent<BoxColliderComponent<Mode>, Mode> {
   public:
    BoxColliderComponent() = default;
    BoxColliderComponent(const AABB& box)
        : box(box) {}

    AABB box;
};

}  // namespace shkyera
