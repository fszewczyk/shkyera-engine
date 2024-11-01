#pragma once

#include <vector>

#include <ECS/Registry.hpp>
#include <Rendering/ShaderProgram.hpp>

namespace shkyera {

class DevelopmentRendererSystem
{
    public:
        DevelopmentRendererSystem(std::shared_ptr<Registry> registry);

        void render();

    private:
        void drawOutline(Entity entity);

        std::shared_ptr<Registry> _registry;
        ShaderProgram _outlineShaderProgram;
};

}
