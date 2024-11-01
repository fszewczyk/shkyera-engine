#pragma once

#include <memory>

#include <Common/Types.hpp>
#include <ECS/Registry.hpp>
#include <Rendering/ShaderProgram.hpp>
#include <Rendering/FrameBuffer.hpp>

namespace shkyera {

class RenderingSystem {
public:
    RenderingSystem(std::shared_ptr<Registry> registry);
    void render();

    void setSize(uint32_t width, uint32_t height);
    GLuint getRenderFrameBuffer();

private:
    void renderModels();
    void renderWireframes();

    std::shared_ptr<Registry> _registry;
    
    FrameBuffer _renderFrameBuffer;
    FrameBuffer _silhouetteFrameBuffer;
    FrameBuffer _blurredSilhouetteFrameBuffer;

    ShaderProgram _modelShaderProgram;
    ShaderProgram _wireframeShaderProgram;
    ShaderProgram _outlineShaderProgram;
    ShaderProgram _blurShaderProgram;
};

}
