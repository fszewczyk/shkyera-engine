#include <Runtime/Runtime.hpp>

#include <Components/NameComponent.hpp>

namespace shkyera {

Runtime::Runtime(std::shared_ptr<Registry> registry) : _registry(registry), _renderer(registry) {
    
}

void Runtime::run() {
    _renderer.draw();
}

const Renderer& Runtime::getRenderer() const {
    return _renderer;
}

Renderer& Runtime::getRenderer() {
    return _renderer;
}


}  // namespace shkyera
