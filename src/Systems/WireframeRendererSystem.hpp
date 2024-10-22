
#pragma once

#include <memory>
#include <ECS/Registry.hpp>
#include <Rendering/ShaderProgram.hpp>


namespace shkyera {

class WireframeRendererSystem {
public:
    WireframeRendererSystem(std::shared_ptr<Registry> registry);
    void render();

private:
    std::shared_ptr<Registry> _registry;
    ShaderProgram _wireframeShaderProgram;
};

}
