#pragma once

#include <glm/glm.hpp>

#include <AssetManager/CubeMap.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class GizmoHandleComponent : public BaseComponent<GizmoHandleComponent> {
   public:
    enum class Direction {
        X,
        Y,
        Z,
        ANY
    };

    GizmoHandleComponent() = default;
    GizmoHandleComponent(Direction dir)
        : direction(dir) {}

    Direction direction = Direction::ANY;
};

}  // namespace shkyera
