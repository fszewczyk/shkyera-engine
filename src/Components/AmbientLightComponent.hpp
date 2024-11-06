#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class AmbientLightComponent : public BaseComponent<AmbientLightComponent> {
public:
    float intensity = 1;
    glm::vec3 color = {0.1, 0.1, 0.1};
};

} // namespace shkyera
