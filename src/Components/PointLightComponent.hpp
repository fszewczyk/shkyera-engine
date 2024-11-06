#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class PointLightComponent : public BaseComponent<PointLightComponent> {
public:
    float intensity = 1;
    float range = 5;
    glm::vec3 color = {1, 1, 1};
};

} // namespace shkyera
