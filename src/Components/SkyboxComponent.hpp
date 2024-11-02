#pragma once

#include <glm/glm.hpp>

#include <Components/BaseComponent.hpp>
#include <Rendering/CubeMap.hpp>

namespace shkyera {

class SkyboxComponent : public BaseComponent<SkyboxComponent> {
public:
    glm::vec3 ambientLight = {0.1, 0.1, 0.1};
    CubeMap skyboxCubeMap;
};

} // namespace shkyera
