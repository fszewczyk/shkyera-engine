#pragma once

#include <iostream>
#include <string>

#include <Components/BaseComponent.hpp>
#include <glm/glm.hpp>

namespace shkyera {

class AmbientLightComponent : public BaseComponent<AmbientLightComponent> {
   public:
    float intensity = 1;
    glm::vec3 color = {0.1, 0.1, 0.1};
};

}  // namespace shkyera
