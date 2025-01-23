#pragma once

#include <memory>

#include <Common/Types.hpp>
#include <ECS/Registry.hpp>
#include <Components/DirectionalLightComponent.hpp>
#include <Rendering/ShaderProgram.hpp>
#include <Rendering/FrameBuffers/SceneFrameBuffer.hpp>
#include <Rendering/FrameBuffers/DepthAtlasFrameBuffer.hpp>
#include <Rendering/CubeMap.hpp>

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
    void renderSkybox();
    void renderGizmo();

    void renderDirectionalLightShadowMaps();
    void renderPointLightShadowMaps();
    void renderSpotLightShadowMaps();

    std::shared_ptr<Registry> _registry;
    
    // Main Rendering
    SceneFrameBuffer _renderFrameBuffer;
    ShaderProgram _modelShaderProgram;
    ShaderProgram _wireframeShaderProgram;

    // Rendering Object Outline
    SceneFrameBuffer _silhouetteFrameBuffer;
    SceneFrameBuffer _horizontallyDilatedFrameBuffer;
    SceneFrameBuffer _fullyDilatedFrameBuffer;
    SceneFrameBuffer _differenceFrameBuffer;
    ShaderProgram _silhouetteShaderProgram;
    ShaderProgram _dilateShaderProgram;
    ShaderProgram _subtractShaderProgram;
    ShaderProgram _overlayShaderProgram;

    // Skybox Rendering
    CubeMap _skyboxCubeMap;
    ShaderProgram _skyboxShaderProgram;

    // Light rendering
    std::unordered_map<Entity, DepthAtlasFrameBuffer> _directionalLightToShadowMap;
    ShaderProgram _shadowMapShaderProgram;

    std::unordered_map<Entity, DepthAtlasFrameBuffer> _pointLightToShadowMap;
    ShaderProgram _shadowMapDistanceShaderProgram;

    std::unordered_map<Entity, DepthAtlasFrameBuffer> _spotLightToShadowMap;
};

}
