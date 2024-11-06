#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class DirectionalLightComponent : public BaseComponent<DirectionalLightComponent> {
public:
    float intensity = 0.5;
    glm::vec3 color = {1.0f, 1.0f, 1.0f};
};

} // namespace shkyera
