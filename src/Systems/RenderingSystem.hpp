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
    void clearFrameBuffers();

    void renderCameraDepth();
    void renderNormals();
    void renderSSAO();
    void renderModels();
    void bloom();
    void toneMapping();
    void renderWireframes();
    void renderOutline(const std::unordered_set<Entity>& entities);
    void renderSkybox();
    void renderOverlayModels();
    void antiAliasing();

    void renderDirectionalLightShadowMaps();
    void renderPointLightShadowMaps();
    void renderSpotLightShadowMaps();

    std::shared_ptr<Registry> _registry;
    
    // Rendering Pipeline
    SceneFrameBuffer* _mostRecentFrameBufferPtr;

    // Main Rendering
    SceneFrameBuffer _litModelsFrameBuffer;
    ShaderProgram _modelShaderProgram;
    ShaderProgram _wireframeShaderProgram;

    // Anti-Aliasing
    SceneFrameBuffer _antiAliasedFrameBuffer;
    ShaderProgram _antiAliasingShaderProgram;

    // Screen-Space Ambient Occlusion
    SceneFrameBuffer _viewSpacePositionFrameBuffer;
    SceneFrameBuffer _viewSpaceNormalFrameBuffer;
    SceneFrameBuffer _ssaoFrameBuffer;
    ShaderProgram _viewSpaceNormalShaderProgram;
    ShaderProgram _viewSpacePositionShaderProgram;
    ShaderProgram _ssaoShaderProgram;

    // Tone Mapping
    SceneFrameBuffer _toneMappedFrameBuffer;
    ShaderProgram _toneMappingShaderProgram;

    // Bloom
    inline static constexpr size_t BloomSteps = 4;
    std::array<SceneFrameBuffer, BloomSteps> _downscaledFrameBuffers;
    std::array<SceneFrameBuffer, BloomSteps> _horizontallyBluredDownscaledFrameBuffers;
    std::array<SceneFrameBuffer, BloomSteps> _fullyBluredDownscaledFrameBuffers;
    SceneFrameBuffer _bloomedFrameBuffer;
    ShaderProgram _thresholdShaderProgram;
    ShaderProgram _horizontalBlurShaderProgram;
    ShaderProgram _verticalBlurShaderProgram;
    ShaderProgram _weightedAdditionShaderProgram;

    // Rendering Object Outline
    SceneFrameBuffer _silhouetteFrameBuffer;
    SceneFrameBuffer _horizontallyDilatedFrameBuffer;
    SceneFrameBuffer _fullyDilatedFrameBuffer;
    SceneFrameBuffer _differenceFrameBuffer;
    SceneFrameBuffer _outlinedObjectsFrameBuffer;
    ShaderProgram _silhouetteShaderProgram;
    ShaderProgram _dilateShaderProgram;
    ShaderProgram _subtractShaderProgram;
    ShaderProgram _overlayShaderProgram;

    // Skybox Rendering
    CubeMap _skyboxCubeMap;
    ShaderProgram _skyboxShaderProgram;

    // Light rendering
    std::unordered_map<Entity, DepthAtlasFrameBuffer> _directionalLightToShadowMap;
    ShaderProgram _depthShaderProgram;

    std::unordered_map<Entity, DepthAtlasFrameBuffer> _pointLightToShadowMap;
    ShaderProgram _distanceShaderProgram;

    std::unordered_map<Entity, DepthAtlasFrameBuffer> _spotLightToShadowMap;
};

}
