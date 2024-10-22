#pragma once

#include <memory>
#include <ECS/Registry.hpp>
#include <Rendering/ShaderProgram.hpp>


namespace shkyera {

class ModelRendererSystem {
public:
    ModelRendererSystem(std::shared_ptr<Registry> registry);
    void render();

private:
    std::shared_ptr<Registry> _registry;
    ShaderProgram _modelShaderProgram;
};

}
