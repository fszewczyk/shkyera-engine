#pragma once

#include <glm/glm.hpp>

#include <Common/Serialization.hpp>
#include <Components/BaseComponent.hpp>
#include <Rendering/CubeMap.hpp>

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

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(direction));
    }
};

}  // namespace shkyera
