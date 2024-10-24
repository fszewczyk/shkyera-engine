#pragma once

#include <memory>
#include <ECS/Registry.hpp>

#include <Systems/CameraMovementSystem.hpp>
#include <Systems/ModelRendererSystem.hpp>
#include <Systems/ObjectSelectionSystem.hpp>
#include <Systems/RenderingSystem.hpp>
#include <Systems/WireframeRendererSystem.hpp>

namespace shkyera {

class Runtime {
public:
    Runtime(std::shared_ptr<Registry> registry);
    void update();

    RenderingSystem& getRenderingSystem();
    const RenderingSystem& getRenderingSystem() const;

private:
    std::shared_ptr<Registry> _registry;
    CameraMovementSystem _cameraMovementSystem;
    RenderingSystem _renderingSystem;
    ModelRendererSystem _modelRendererSystem;
    WireframeRendererSystem _wireframeRendererSystem;
};

}
