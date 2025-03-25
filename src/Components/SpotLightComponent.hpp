#pragma once

#include <iostream>
#include <string>

#include <Common/Serialization.hpp>
#include <Components/BaseComponent.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "cereal/cereal.hpp"

namespace shkyera {

class SpotLightComponent : public BaseComponent<SpotLightComponent> {
   public:
    float intensity = 1;
    float range = 5;
    float innerCutoff = glm::radians(15.0f);
    float outerCutoff = glm::radians(25.0f);
    glm::vec3 color = {1, 1, 1};

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(intensity));
        archive(CEREAL_NVP(range));
        archive(CEREAL_NVP(innerCutoff));
        archive(CEREAL_NVP(outerCutoff));
        archive(CEREAL_NVP(color));
    }

    glm::mat4 getLightSpaceMatrix(const glm::mat4& transformMatrix) const {
        const auto lightPosition = glm::vec3{transformMatrix[3]};
        const auto projection = glm::perspective(2 * outerCutoff, 1.0f, 0.01f, range);
        const auto lightDirection = glm::normalize(glm::vec3{1, 0, 0} * glm::mat3{glm::inverse(transformMatrix)});
        const auto lightSpaceMatrix = projection * glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3{0, 1, 0});
        return lightSpaceMatrix;
    }
};

}  // namespace shkyera
