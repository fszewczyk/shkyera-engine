#pragma once

#include <memory>

#include <AssetManager/Material.hpp>
#include <Common/Types.hpp>
#include <ECS/Registry.hpp>
#include <Components/DirectionalLightComponent.hpp>
#include <Components/PostProcessingVolumeComponent.hpp>
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

    // Supporting Textures
    void renderViewPosition();
    void renderViewNormals();
    void renderSSAO(float strength, float radius);
    void renderDirectionalLightShadowMaps();
    void renderPointLightShadowMaps();
    void renderSpotLightShadowMaps();

    // Main Rendering
    void renderWorldObjects(const PostProcessingVolumeComponent& postProcessing);
    void renderParticles();
    void renderBillboards();
    void renderPostProcessingVolumes();
    void renderWireframes();
    void renderOutline(const std::unordered_set<Entity>& entities);
    void renderSkybox();
    void renderOverlayModels();

    // Post-Processing
    void bloom(float threshold, float weight);
    void toneMapping();
    void gammaCorrection(float gamma);
    void antiAliasing();

    // Helper methods
    void setMaterial(Material const* material);
    void setTexture(Material const* material, const std::string& textureName, auto textureMember);
    PostProcessingVolumeComponent getPostProcessingSettings();
    int _textureIndex;

    // Data
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
    ShaderProgram _toneMappingShaderProgram;
    SceneFrameBuffer _toneMappedFrameBuffer;

    // Gamma Correction
    ShaderProgram _gammaCorrectionShaderProgram;
    SceneFrameBuffer _gammaCorrectedFrameBuffer;

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

    // Debug Rendering
    Material _defaultMaterial;

    Material _directionalLightDebugMaterial;
    Material _spotLightDebugMaterial;
    Material _pointLightDebugMaterial;
    Material _ambientLightDebugMaterial;
    Material _particleEmitterDebugMaterial;
    Material _postProcessingVolumeDebugMaterial;
};

}
