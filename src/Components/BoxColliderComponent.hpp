#pragma once

#include <string>

#include <Components/BaseComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Math/AABB.hpp>

namespace shkyera {

template <RuntimeMode Mode = RuntimeMode::PRODUCTION>
struct BoxColliderComponent {
  AABB box;
};

}  // namespace shkyera
