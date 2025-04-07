#pragma once

#include <Components/BaseComponent.hpp>
#include <glm/glm.hpp>

namespace shkyera {

struct AmbientLightComponent {
    float intensity = 1;
    glm::vec3 color = {0.1, 0.1, 0.1};
};

}  // namespace shkyera
