#pragma once

#include <memory>
#include <ECS/Registry.hpp>
#include <Common/Types.hpp>
#include <Common/Clock.hpp>

#include <Systems/CameraMovementSystem.hpp>
#include <Systems/ParticleSystem.hpp>
#include <Systems/RenderingSystem.hpp>
#include <Rendering/FrameBuffers/SceneFrameBuffer.hpp>

namespace shkyera {

template<RuntimeMode Mode>
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

    RenderingSystem& getRenderingSystem() {
        return _renderingSystem;
    }

    const RenderingSystem& getRenderingSystem() const {
        return _renderingSystem;
    }


private:
    std::shared_ptr<Registry> _registry;

    CameraMovementSystem _cameraMovementSystem;
    ParticleSystem _particleSystem;
    RenderingSystem _renderingSystem;
};

}
