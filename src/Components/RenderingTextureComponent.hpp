#pragma once

#include <Components/BaseComponent.hpp>
#include <ECS/SingletonComponent.hpp>

#include <optional>

namespace shkyera {

struct RenderingTextureComponent : public BaseComponent<RenderingTextureComponent> {
  float renderWidth{128};
  float renderHeight{128};

  std::optional<uint32_t> renderedTextureId{};
};

struct SceneCamera : public RenderingTextureComponent, SingletonComponent<SceneCamera> {};

struct RuntimeCamera : public RenderingTextureComponent, SingletonComponent<RuntimeCamera> {};

}  // namespace shkyera
