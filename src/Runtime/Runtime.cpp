#include <Runtime/Runtime.hpp>

#include <Components/TextureComponent.hpp>

namespace shkyera {

Runtime::Runtime(std::shared_ptr<Registry> registry) : _registry(registry) {}

void Runtime::run() {
    for(auto& comp : _registry->getComponents<TextureComponent>()) {
        comp.update();
    }
}

}  // namespace shkyera
