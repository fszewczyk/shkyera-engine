#pragma once

#include <ECS/SingletonComponent.hpp>
#include <optional>

namespace shkyera {

struct CameraTag {
  float renderWidth{128};
  float renderHeight{128};

  std::optional<uint32_t> renderedTextureId{};
};

struct SceneCamera : public SingletonComponent, public CameraTag {};

struct RuntimeCamera : public SingletonComponent, public CameraTag {};

}  // namespace shkyera
