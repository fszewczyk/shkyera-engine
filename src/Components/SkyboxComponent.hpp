#pragma once

#include <glm/glm.hpp>

#include <Components/BaseComponent.hpp>
#include <Rendering/CubeMap.hpp>

namespace shkyera {

class SkyboxComponent : public BaseComponent<SkyboxComponent> {
   public:
    CubeMap skyboxCubeMap;
};

}  // namespace shkyera
