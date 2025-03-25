#pragma once

#include <iostream>
#include <string>

#include <Common/Serialization.hpp>
#include <Components/BaseComponent.hpp>
#include <glm/glm.hpp>
#include "cereal/cereal.hpp"

namespace shkyera {

class PointLightComponent : public BaseComponent<PointLightComponent> {
   public:
    float intensity = 1;
    float range = 5;
    glm::vec3 color = {1, 1, 1};

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(intensity));
        archive(CEREAL_NVP(range));
        archive(CEREAL_NVP(color));
    }
};

}  // namespace shkyera
