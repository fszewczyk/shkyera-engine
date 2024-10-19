#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class PointLightComponent : public BaseComponent<PointLightComponent> {
public:
    glm::vec3 ambient = {0.5, 0.5, 0.5};
    glm::vec3 diffuse = {0.5, 0.5, 0.5};
    glm::vec3 specular = {0.5, 0.5, 0.5};
};

} // namespace shkyera
