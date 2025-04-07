#pragma once

#include <glm/glm.hpp>

#include <AssetManager/CubeMap.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

struct SkyboxComponent {
  HandleAndAsset<CubeMap> skyboxCubeMap;
};

}  // namespace shkyera
