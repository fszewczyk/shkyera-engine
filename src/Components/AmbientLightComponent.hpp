#pragma once

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <Common/Serialization.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class AmbientLightComponent : public BaseComponent<AmbientLightComponent> {
public:
    float intensity = 1;
    glm::vec3 color = {0.1, 0.1, 0.1};

    template<typename Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(intensity));
        archive(CEREAL_NVP(color));
    }
};

} // namespace shkyera
