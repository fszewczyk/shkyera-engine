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
    void renderOutline(const std::unordered_set<Entity>& entities);

    std::shared_ptr<Registry> _registry;
    
    // Main Rendering
    FrameBuffer _renderFrameBuffer;
    ShaderProgram _modelShaderProgram;
    ShaderProgram _wireframeShaderProgram;

    // Rendering Object Outline
    FrameBuffer _silhouetteFrameBuffer;
    FrameBuffer _horizontallyDilatedFrameBuffer;
    FrameBuffer _fullyDilatedFrameBuffer;
    FrameBuffer _differenceFrameBuffer;

    ShaderProgram _silhouetteShaderProgram;
    ShaderProgram _dilateShaderProgram;
    ShaderProgram _subtractShaderProgram;
    ShaderProgram _overlayShaderProgram;
};

}
