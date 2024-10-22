#include <Runtime/Runtime.hpp>
#include <InputManager/InputManager.hpp>
#include <iostream>

namespace shkyera {

Runtime::Runtime(std::shared_ptr<Registry> registry)
    : _registry(registry),
      _cameraMovementSystem(registry),
      _modelRendererSystem(registry),
      _renderingSystem(registry),
      _wireframeRendererSystem(registry) {}

void Runtime::update() {
    _cameraMovementSystem.update();

    _renderingSystem.startFrame();
    _modelRendererSystem.render();
    _wireframeRendererSystem.render();

    _renderingSystem.endFrame();
}

const RenderingSystem& Runtime::getRenderingSystem() const {
    return _renderingSystem;
}

RenderingSystem& Runtime::getRenderingSystem() {
    return _renderingSystem;
}

}
