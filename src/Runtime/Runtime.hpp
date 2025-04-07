#pragma once

#include <memory>

#include <Common/Clock.hpp>
#include <Common/Types.hpp>
#include <Components/SceneCamera.hpp>
#include <ECS/Registry.hpp>
#include <Rendering/FrameBuffers/SceneFrameBuffer.hpp>
#include <Systems/CameraMovementSystem.hpp>
#include <Systems/ParticleSystem.hpp>
#include <Systems/RenderingSystem.hpp>

namespace shkyera {

template <typename MainCameraTag>
class Runtime {
   public:
    Runtime(std::shared_ptr<Registry> registry)
        : _registry(registry),
          _cameraMovementSystem(registry),
          _particleSystem(registry),
          _renderingSystem(registry) {}

    void update() {
        clock::Game.reset();

        _cameraMovementSystem.update();
        _particleSystem.update();
        _renderingSystem.render();
    }

    auto& getRenderingSystem() {
        return _renderingSystem;
    }

    const auto& getRenderingSystem() const {
        return _renderingSystem;
    }

   private:
    std::shared_ptr<Registry> _registry;

    CameraMovementSystem<MainCameraTag> _cameraMovementSystem;
    ParticleSystem _particleSystem;
    RenderingSystem<MainCameraTag> _renderingSystem;
};

}  // namespace shkyera
