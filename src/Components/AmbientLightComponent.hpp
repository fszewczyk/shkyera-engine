#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class AmbientLightComponent : public BaseComponent<AmbientLightComponent> {
public:
    glm::vec3 ambient = {0.1, 0.1, 0.1};
};

} // namespace shkyera
