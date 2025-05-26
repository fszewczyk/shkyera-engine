#pragma once

#include <chrono>
#include <memory>
#include <random>

#include <AssetManager/CubeMap.hpp>
#include <AssetManager/Material.hpp>
#include <AssetManager/Texture.hpp>
#include <Common/Assert.hpp>
#include <Common/Logger.hpp>
#include <Common/Profiler.hpp>
#include <Common/Random.hpp>
#include <Common/Types.hpp>
#include <Components/AmbientLightComponent.hpp>
#include <Components/BillboardComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/CameraTags.hpp>
#include <Components/DirectionalLightComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/OverlayModelComponent.hpp>
#include <Components/ParticleEmitterComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <Components/PostProcessingVolumeComponent.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/SpotLightComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <ECS/Registry.hpp>
#include <ECS/RegistryViewer.hpp>
#include <Math/AABB.hpp>
#include <Rendering/FrameBuffers/DepthAtlasFrameBuffer.hpp>
#include <Rendering/FrameBuffers/SceneFrameBuffer.hpp>
#include <Rendering/ShaderProgram.hpp>
#include <Rendering/Utils.hpp>
#include <Systems/RenderingSystem.hpp>
#include <Utils/AssetUtils.hpp>
#include <Utils/TransformUtils.hpp>

#include <type_traits>
#include "Components/SelectedEntityComponent.hpp"

namespace shkyera {

template <typename MainCameraTag>
class RenderingSystem : public RegistryViewer {
 public:
  RenderingSystem(std::shared_ptr<Registry> registry);
  std::optional<GLuint> update();

  GLuint getRenderFrameBuffer();

 private:
  void clearFrameBuffers();
  void setSize(uint32_t width, uint32_t height);

  // Supporting Textures
  void renderViewPosition();
  void renderViewNormals();
  void renderSSAO(float strength, float radius);
  void renderDirectionalLightShadowMaps();
  void renderPointLightShadowMaps();
  void renderSpotLightShadowMaps();

  // Main Rendering
  void renderWorldObjects();
  void renderParticles();
  void renderBillboards();
  void renderDebugBillboards();
  void renderPostProcessingVolumes();
  void renderWireframes();
  void renderOutlineOfSelectedEntities();
  void renderOutline(const std::set<Entity>& entities);
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
  std::vector<glm::vec3> _ssaoKernel;
  float _usedSsaoRadius{0};

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

template <typename MainCameraTag>
RenderingSystem<MainCameraTag>::RenderingSystem(std::shared_ptr<Registry> registry)
    : RegistryViewer(registry,
                     ReadAccess<TransformComponent, ModelComponent, NameComponent, EntityHierarchy, SkyboxComponent,
                                DirectionalLightComponent, PointLightComponent, SpotLightComponent,
                                BillboardComponent<>, WireframeComponent, OverlayModelComponent, AmbientLightComponent,
                                ParticleEmitterComponent, PostProcessingVolumeComponent>(),
                     WriteAccess<MainCameraTag, CameraComponent>()),
      _registry(registry) {
  const auto& positionAndNormalVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/position_and_normal.glsl", Shader::Type::Vertex);
  const auto& modelFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/uber.glsl", Shader::Type::Fragment);
  _modelShaderProgram.attachShader(positionAndNormalVertexShader);
  _modelShaderProgram.attachShader(modelFragmentShader);
  _modelShaderProgram.link();

  const auto& distanceVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/distance.glsl", Shader::Type::Vertex);
  const auto& distanceFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/distance.glsl", Shader::Type::Fragment);
  _distanceShaderProgram.attachShader(distanceVertexShader);
  _distanceShaderProgram.attachShader(distanceFragmentShader);
  _distanceShaderProgram.link();

  const auto& viewSpaceNormalVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/viewspace_normal.glsl", Shader::Type::Vertex);
  const auto& viewSpaceNormalFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/viewspace_normal.glsl", Shader::Type::Fragment);
  _viewSpaceNormalShaderProgram.attachShader(viewSpaceNormalVertexShader);
  _viewSpaceNormalShaderProgram.attachShader(viewSpaceNormalFragmentShader);
  _viewSpaceNormalShaderProgram.link();

  const auto& viewSpaceDepthVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/viewspace_position.glsl", Shader::Type::Vertex);
  const auto& viewSpaceDepthFragmentShader = utils::assets::readPermanent<Shader>(
      "resources/shaders/fragment/viewspace_position.glsl", Shader::Type::Fragment);
  _viewSpacePositionShaderProgram.attachShader(viewSpaceDepthVertexShader);
  _viewSpacePositionShaderProgram.attachShader(viewSpaceDepthFragmentShader);
  _viewSpacePositionShaderProgram.link();

  const auto& texCoordsVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/texcoords.glsl", Shader::Type::Vertex);
  const auto& ssaoFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/ssao.glsl", Shader::Type::Fragment);
  _ssaoShaderProgram.attachShader(texCoordsVertexShader);
  _ssaoShaderProgram.attachShader(ssaoFragmentShader);
  _ssaoShaderProgram.link();

  const auto& toneMappingFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/tonemapping_aces.glsl", Shader::Type::Fragment);
  _toneMappingShaderProgram.attachShader(texCoordsVertexShader);
  _toneMappingShaderProgram.attachShader(toneMappingFragmentShader);
  _toneMappingShaderProgram.link();

  const auto& gammaCorrectionFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/gamma_correction.glsl", Shader::Type::Fragment);
  _gammaCorrectionShaderProgram.attachShader(texCoordsVertexShader);
  _gammaCorrectionShaderProgram.attachShader(gammaCorrectionFragmentShader);
  _gammaCorrectionShaderProgram.link();

  const auto& luminosityThresholdFragmentShader = utils::assets::readPermanent<Shader>(
      "resources/shaders/fragment/luminosity_threshold.glsl", Shader::Type::Fragment);
  _thresholdShaderProgram.attachShader(texCoordsVertexShader);
  _thresholdShaderProgram.attachShader(luminosityThresholdFragmentShader);
  _thresholdShaderProgram.link();

  const auto& horizontalGaussianBlur5FragmentShader = utils::assets::readPermanent<Shader>(
      "resources/shaders/fragment/horizontal_gaussianblur_5.glsl", Shader::Type::Fragment);
  _horizontalBlurShaderProgram.attachShader(texCoordsVertexShader);
  _horizontalBlurShaderProgram.attachShader(horizontalGaussianBlur5FragmentShader);
  _horizontalBlurShaderProgram.link();

  const auto& verticalGaussianBlur5FragmentShader = utils::assets::readPermanent<Shader>(
      "resources/shaders/fragment/vertical_gaussianblur_5.glsl", Shader::Type::Fragment);
  _verticalBlurShaderProgram.attachShader(texCoordsVertexShader);
  _verticalBlurShaderProgram.attachShader(verticalGaussianBlur5FragmentShader);
  _verticalBlurShaderProgram.link();

  const auto& weightedAdditionFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/weighted_addition.glsl", Shader::Type::Fragment);
  _weightedAdditionShaderProgram.attachShader(texCoordsVertexShader);
  _weightedAdditionShaderProgram.attachShader(weightedAdditionFragmentShader);
  _weightedAdditionShaderProgram.link();

  const auto& positionVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/position.glsl", Shader::Type::Vertex);
  const auto& fixedColorFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/color.glsl", Shader::Type::Fragment);
  _wireframeShaderProgram.attachShader(positionVertexShader);
  _wireframeShaderProgram.attachShader(fixedColorFragmentShader);
  _wireframeShaderProgram.link();

  _silhouetteShaderProgram.attachShader(positionVertexShader);
  _silhouetteShaderProgram.attachShader(fixedColorFragmentShader);
  _silhouetteShaderProgram.link();

  const auto& dilateFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/dilate.glsl", Shader::Type::Fragment);
  _dilateShaderProgram.attachShader(texCoordsVertexShader);
  _dilateShaderProgram.attachShader(dilateFragmentShader);
  _dilateShaderProgram.link();

  const auto& subtractFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/subtract.glsl", Shader::Type::Fragment);
  _subtractShaderProgram.attachShader(texCoordsVertexShader);
  _subtractShaderProgram.attachShader(subtractFragmentShader);
  _subtractShaderProgram.link();

  const auto& overlayFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/overlay.glsl", Shader::Type::Fragment);
  _overlayShaderProgram.attachShader(texCoordsVertexShader);
  _overlayShaderProgram.attachShader(overlayFragmentShader);
  _overlayShaderProgram.link();

  const auto& fxaaFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/fxaa.glsl", Shader::Type::Fragment);
  _antiAliasingShaderProgram.attachShader(texCoordsVertexShader);
  _antiAliasingShaderProgram.attachShader(fxaaFragmentShader);
  _antiAliasingShaderProgram.link();

  const auto& skyboxVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/skybox.glsl", Shader::Type::Vertex);
  const auto& cubeMapFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/cubemap.glsl", Shader::Type::Fragment);
  _skyboxShaderProgram.attachShader(skyboxVertexShader);
  _skyboxShaderProgram.attachShader(cubeMapFragmentShader);
  _skyboxShaderProgram.link();

  const auto& depthVertexShader =
      utils::assets::readPermanent<Shader>("resources/shaders/vertex/depth.glsl", Shader::Type::Vertex);
  const auto& depthFragmentShader =
      utils::assets::readPermanent<Shader>("resources/shaders/fragment/depth.glsl", Shader::Type::Fragment);
  _depthShaderProgram.attachShader(depthVertexShader);
  _depthShaderProgram.attachShader(depthFragmentShader);
  _depthShaderProgram.link();

  _defaultMaterial.lit = false;
  _defaultMaterial.albedo = glm::vec3{1, 0, 1};

  Material debugBillboardMaterial;
  debugBillboardMaterial.lit = false;
  _particleEmitterDebugMaterial = _directionalLightDebugMaterial = _spotLightDebugMaterial = _pointLightDebugMaterial =
      _ambientLightDebugMaterial = _postProcessingVolumeDebugMaterial = debugBillboardMaterial;

  std::get<AssetRef<Texture>>(_pointLightDebugMaterial.albedoTexture) =
      utils::assets::readPermanent<Texture>("resources/icons/components/point_light.png");
  std::get<AssetRef<Texture>>(_spotLightDebugMaterial.albedoTexture) =
      utils::assets::readPermanent<Texture>("resources/icons/components/spot_light.png");
  std::get<AssetRef<Texture>>(_directionalLightDebugMaterial.albedoTexture) =
      utils::assets::readPermanent<Texture>("resources/icons/components/directional_light.png");
  std::get<AssetRef<Texture>>(_ambientLightDebugMaterial.albedoTexture) =
      utils::assets::readPermanent<Texture>("resources/icons/components/ambient_light.png");
  std::get<AssetRef<Texture>>(_particleEmitterDebugMaterial.albedoTexture) =
      utils::assets::readPermanent<Texture>("resources/icons/components/particles_gray.png");
  std::get<AssetRef<Texture>>(_postProcessingVolumeDebugMaterial.albedoTexture) =
      utils::assets::readPermanent<Texture>("resources/icons/components/post_processing_gray.png");
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::setSize(uint32_t width, uint32_t height) {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::setSize");

  if (const auto cameraOpt = _registry->getEntity<MainCameraTag>()) {
    const auto aspectRatio = static_cast<float>(width) / height;
    RegistryViewer::write<CameraComponent>(*cameraOpt).aspectRatio = aspectRatio;
  }

  _litModelsFrameBuffer.setSize(width, height);
  _silhouetteFrameBuffer.setSize(width, height);
  _horizontallyDilatedFrameBuffer.setSize(width, height);
  _fullyDilatedFrameBuffer.setSize(width, height);
  _toneMappedFrameBuffer.setSize(width, height);
  _gammaCorrectedFrameBuffer.setSize(width, height);
  _differenceFrameBuffer.setSize(width, height);
  _bloomedFrameBuffer.setSize(width, height);
  _antiAliasedFrameBuffer.setSize(width, height);
  _outlinedObjectsFrameBuffer.setSize(width, height);
  _viewSpaceNormalFrameBuffer.setSize(width, height);
  _viewSpacePositionFrameBuffer.setSize(width, height);
  _ssaoFrameBuffer.setSize(width, height);

  size_t downscaleFactor = 2;
  for (size_t i = 0; i < BloomSteps; ++i) {
    _downscaledFrameBuffers[i].setSize(width / downscaleFactor, height / downscaleFactor);
    _horizontallyBluredDownscaledFrameBuffers[i].setSize(width / downscaleFactor, height / downscaleFactor);
    _fullyBluredDownscaledFrameBuffers[i].setSize(width / downscaleFactor, height / downscaleFactor);
    downscaleFactor *= 1.5;
  }
}

template <typename MainCameraTag>
GLuint RenderingSystem<MainCameraTag>::getRenderFrameBuffer() {
  return _mostRecentFrameBufferPtr->getTexture().getID();
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::clearFrameBuffers() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::clearFrameBuffers");

  _litModelsFrameBuffer.clear();
  _bloomedFrameBuffer.clear();
  _antiAliasedFrameBuffer.clear();
  _outlinedObjectsFrameBuffer.clear();
  _silhouetteFrameBuffer.clear();
  _horizontallyDilatedFrameBuffer.clear();
  _fullyDilatedFrameBuffer.clear();
  _differenceFrameBuffer.clear();
  _toneMappedFrameBuffer.clear();
  _gammaCorrectedFrameBuffer.clear();
  _viewSpaceNormalFrameBuffer.clear();
  _viewSpacePositionFrameBuffer.clear();
  _ssaoFrameBuffer.clear(glm::vec3{1.0, 1.0, 1.0});

  for (auto& buffer : _downscaledFrameBuffers) {
    buffer.clear();
  }
  for (auto& buffer : _horizontallyBluredDownscaledFrameBuffers) {
    buffer.clear();
  }
  for (auto& buffer : _fullyBluredDownscaledFrameBuffers) {
    buffer.clear();
  }
}

template <typename MainCameraTag>
std::optional<GLuint> RenderingSystem<MainCameraTag>::update() {
  auto* mainCameraComponent = RegistryViewer::write<MainCameraTag>();

  if (!mainCameraComponent) {
    Logger::ERROR("There is no Camera with registered type. Cannot render anything");
    return std::nullopt;
  }

  setSize(mainCameraComponent->renderWidth, mainCameraComponent->renderHeight);

  constexpr auto DrawDebugInfo = std::is_same_v<MainCameraTag, SceneCamera>;

  _textureIndex = 0;
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::render");

  _mostRecentFrameBufferPtr = &_litModelsFrameBuffer;
  const auto postProcessing = getPostProcessingSettings();

  // Rendering Preparation
  clearFrameBuffers();

  // Rendering Supporting Textures
  renderViewPosition();
  renderViewNormals();
  renderSSAO(postProcessing.ssaoStrength, postProcessing.ssaoRadius);
  renderDirectionalLightShadowMaps();
  renderPointLightShadowMaps();
  renderSpotLightShadowMaps();

  // Main Rendering Pass
  renderSkybox();
  renderWorldObjects();
  renderParticles();
  renderBillboards();
  renderWireframes();

  // Post-Processing
  if (postProcessing.bloomWeight > 0.0f)
    bloom(postProcessing.bloomThreshold, postProcessing.bloomWeight);
  if (postProcessing.toneMapping)
    toneMapping();
  if (postProcessing.gamma != 1.0f)
    gammaCorrection(postProcessing.gamma);
  if (postProcessing.antiAliasing)
    antiAliasing();

  if constexpr (DrawDebugInfo) {
    renderPostProcessingVolumes();
    renderOutlineOfSelectedEntities();
    renderDebugBillboards();
    renderOverlayModels();
  }

  mainCameraComponent->renderedTextureId = _mostRecentFrameBufferPtr->getTexture().getID();

  return mainCameraComponent->renderedTextureId;
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderViewPosition() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderViewPosition");

  _viewSpacePositionShaderProgram.use();
  _viewSpacePositionFrameBuffer.bind();

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraComponent = RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>());

  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();

  _viewSpacePositionShaderProgram.setUniform("projection", projectionMatrix);
  _viewSpacePositionShaderProgram.setUniform("view", viewMatrix);

  for (const auto& [entity, modelComponent] : RegistryViewer::readAll<ModelComponent>()) {
    const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
    _viewSpacePositionShaderProgram.setUniform("model", transformMatrix);

    modelComponent.updateImpl();
  }

  _viewSpacePositionFrameBuffer.unbind();
  _viewSpacePositionShaderProgram.stopUsing();
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderViewNormals() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderViewNormals");

  _viewSpaceNormalShaderProgram.use();
  _viewSpaceNormalFrameBuffer.bind();

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraComponent = RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>());

  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();

  _viewSpaceNormalShaderProgram.setUniform("projection", projectionMatrix);
  _viewSpaceNormalShaderProgram.setUniform("view", viewMatrix);

  for (const auto& [entity, modelComponent] : RegistryViewer::readAll<ModelComponent>()) {
    const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
    _viewSpaceNormalShaderProgram.setUniform("model", transformMatrix);

    modelComponent.updateImpl();
  }

  _viewSpaceNormalFrameBuffer.unbind();
  _viewSpaceNormalShaderProgram.stopUsing();
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderSSAO(float strength, float radius) {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderSSAO");

  constexpr int SSAOSamples = 32;

  bool updatedKernel = false;
  if (_ssaoKernel.empty() || radius != _usedSsaoRadius) {
    updatedKernel = true;
    _usedSsaoRadius = radius;

    _ssaoKernel.resize(SSAOSamples);
    random::Uniform<float> sampler(0.0f, 1.0f);
    for (int i = 0; i < SSAOSamples; ++i) {
      glm::vec3 sample(sampler() * 2.0 - 1.0, sampler() * 2.0 - 1.0, sampler() * 0.95 + 0.05);
      sample = glm::normalize(sample);

      float scale = (float)i / SSAOSamples;
      scale = glm::mix(0.05f, _usedSsaoRadius, scale * scale);
      sample *= scale;

      _ssaoKernel[i] = sample;
    }
  }

  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();

  UseShader ssaoShaderUsage(_ssaoShaderProgram);

  _ssaoShaderProgram.setUniform("strength", strength);

  _viewSpacePositionFrameBuffer.getTexture().activate(GL_TEXTURE0);
  _ssaoShaderProgram.setUniform("depthTexture", 0);
  _viewSpaceNormalFrameBuffer.getTexture().activate(GL_TEXTURE1);
  _ssaoShaderProgram.setUniform("normalTexture", 1);

  if (updatedKernel) {
    for (int i = 0; i < SSAOSamples; i++) {
      _ssaoShaderProgram.setUniform("samples[" + std::to_string(i) + "]", _ssaoKernel[i]);
    }
  }

  // Set uniforms using the parameter pack
  _ssaoShaderProgram.setUniform("projection", projectionMatrix);

  // Draw fullscreen quad
  _ssaoFrameBuffer.bind();
  utils::drawFullscreenQuad();
  _ssaoFrameBuffer.unbind();
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderOutlineOfSelectedEntities() {
  std::set<Entity> selectedEntities;
  for (const auto& [entity, _] : RegistryViewer::readAll<SelectedEntityComponent>()) {
    selectedEntities.insert(entity);
  }
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderOutline(const std::set<Entity>& entities) {
  if (std::none_of(entities.begin(), entities.end(),
                   [this](auto e) { return _registry->hasComponent<NameComponent>(e); })) {
    return;
  }

  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderOutline");

  for (const auto& entity : entities) {
    const auto& children = _registry->getHierarchy().getChildren(entity);
    renderOutline(children);
  }

  glDisable(GL_DEPTH_TEST);

  // Drawing a silhouette
  _silhouetteFrameBuffer.bind();
  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();
  const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

  _silhouetteShaderProgram.use();
  _silhouetteShaderProgram.setUniform("fixedColor", glm::vec3{1.0, 0.1, 1.0});
  for (const auto& entity : entities) {
    if (_registry->hasComponents<TransformComponent, ModelComponent>(entity)) {
      const auto& modelComponent = RegistryViewer::read<ModelComponent>(entity);
      const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
      _silhouetteShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
      modelComponent.updateImpl();
    }
  }
  _silhouetteShaderProgram.stopUsing();
  _silhouetteFrameBuffer.unbind();

  utils::applyShaderToFrameBuffer(_horizontallyDilatedFrameBuffer, _dilateShaderProgram,
                                  {{"silhouetteTexture", &_silhouetteFrameBuffer.getTexture()}},
                                  utils::Uniform("horizontal", true), utils::Uniform("kernelSize", 3));

  utils::applyShaderToFrameBuffer(_fullyDilatedFrameBuffer, _dilateShaderProgram,
                                  {{"silhouetteTexture", &_horizontallyDilatedFrameBuffer.getTexture()}},
                                  utils::Uniform("horizontal", false), utils::Uniform("kernelSize", 3));

  utils::applyShaderToFrameBuffer(
      _differenceFrameBuffer, _subtractShaderProgram,
      {{"first", &_fullyDilatedFrameBuffer.getTexture()}, {"second", &_silhouetteFrameBuffer.getTexture()}});

  utils::applyShaderToFrameBuffer(
      _outlinedObjectsFrameBuffer, _overlayShaderProgram,
      {{"background", &_mostRecentFrameBufferPtr->getTexture()}, {"overlay", &_differenceFrameBuffer.getTexture()}});

  glEnable(GL_DEPTH_TEST);

  _mostRecentFrameBufferPtr = &_outlinedObjectsFrameBuffer;
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderDirectionalLightShadowMaps() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderDirectionalLightShadowMaps");

  glEnable(GL_DEPTH_TEST);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraComponent = RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>());

  std::unordered_set<Entity> directionalLightEntities;
  for (const auto& [entity, directionalLightComponent] : RegistryViewer::readAll<DirectionalLightComponent>()) {
    directionalLightEntities.insert(entity);
    if (_directionalLightToShadowMap.find(entity) == _directionalLightToShadowMap.end()) {
      _directionalLightToShadowMap.emplace(entity, DepthAtlasFrameBuffer(DirectionalLightComponent::LevelsOfDetail));
      auto& newShadowMap = _directionalLightToShadowMap.at(entity);

      newShadowMap.bind();
      newShadowMap.setSize(DirectionalLightComponent::LevelsOfDetail * 2048, 2048);
      newShadowMap.clear();
      newShadowMap.unbind();
    }
  }

  std::vector<Entity> entitiesWithFrameBuffersToRemove;
  for (const auto& [entity, _buffer] : _directionalLightToShadowMap) {
    if (directionalLightEntities.find(entity) == directionalLightEntities.end()) {
      entitiesWithFrameBuffersToRemove.push_back(entity);
    }
  }

  for (const auto& entityToRemove : entitiesWithFrameBuffersToRemove) {
    _directionalLightToShadowMap.erase(entityToRemove);
  }

  for (auto& [lightEntity, shadowMapAtlas] : _directionalLightToShadowMap) {
    shadowMapAtlas.bind();
    shadowMapAtlas.clear();
    shadowMapAtlas.unbind();

    const auto& directionalLightComponent = RegistryViewer::read<DirectionalLightComponent>(lightEntity);
    const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(lightEntity, _registry);

    for (uint8_t levelOfDetail = 0; levelOfDetail < DirectionalLightComponent::LevelsOfDetail; ++levelOfDetail) {
      shadowMapAtlas.bind(levelOfDetail);
      _depthShaderProgram.use();

      const glm::mat4& lightSpaceMatrix = directionalLightComponent.getLightSpaceMatrix(
          lightTransformMatrix, cameraTransform, cameraComponent, levelOfDetail);

      _depthShaderProgram.setUniform("projectionViewMatrix", lightSpaceMatrix);
      for (const auto& [modelEntity, modelComponent] : RegistryViewer::readAll<ModelComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
        _depthShaderProgram.setUniform("modelMatrix", transformMatrix);

        modelComponent.updateImpl();
      }

      _depthShaderProgram.stopUsing();
      shadowMapAtlas.unbind();
    }
  }
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderPointLightShadowMaps() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderPointLightShadowMaps");

  glEnable(GL_DEPTH_TEST);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraComponent = RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>());

  std::unordered_set<Entity> pointLightEntities;
  for (const auto& [entity, pointLightComponent] : RegistryViewer::readAll<PointLightComponent>()) {
    pointLightEntities.insert(entity);
    if (_pointLightToShadowMap.find(entity) == _pointLightToShadowMap.end()) {
      _pointLightToShadowMap.emplace(entity, DepthAtlasFrameBuffer(6));
      auto& newShadowMap = _pointLightToShadowMap.at(entity);

      newShadowMap.bind();
      newShadowMap.setSize(6 * 1024, 1024);
      newShadowMap.clear();
      newShadowMap.unbind();
    }
  }

  std::vector<Entity> entitiesWithFrameBuffersToRemove;
  for (const auto& [entity, _buffer] : _pointLightToShadowMap) {
    if (pointLightEntities.find(entity) == pointLightEntities.end()) {
      entitiesWithFrameBuffersToRemove.push_back(entity);
    }
  }

  for (const auto& entityToRemove : entitiesWithFrameBuffersToRemove) {
    _pointLightToShadowMap.erase(entityToRemove);
  }

  for (auto& [lightEntity, shadowMapCubeAtlas] : _pointLightToShadowMap) {
    shadowMapCubeAtlas.bind();
    shadowMapCubeAtlas.clear();
    shadowMapCubeAtlas.unbind();

    // TODO: Use global position
    const auto& lightPosition = RegistryViewer::read<TransformComponent>(lightEntity).getPosition();
    const auto& lightRange = RegistryViewer::read<PointLightComponent>(lightEntity).range;

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, lightRange);
    glm::mat4 captureViews[6] = {
        projection * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, -1.0f, 0.0f)),  // +X
        projection * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, -1.0f, 0.0f)),  // -X
        projection *
            glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),  // +Y
        projection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, -1.0f)),  // -Y
        projection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f),
                                 glm::vec3(0.0f, -1.0f, 0.0f)),  // +Z
        projection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f),
                                 glm::vec3(0.0f, -1.0f, 0.0f))  // -Z
    };

    _distanceShaderProgram.use();

    for (uint8_t face = 0; face < 6; ++face) {
      shadowMapCubeAtlas.bind(face);

      const glm::mat4& lightSpaceMatrix = captureViews[face];
      _distanceShaderProgram.setUniform("projectionViewMatrix", lightSpaceMatrix);

      for (const auto& [modelEntity, modelComponent] : RegistryViewer::readAll<ModelComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
        _distanceShaderProgram.setUniform("modelMatrix", transformMatrix);

        modelComponent.updateImpl();
      }

      shadowMapCubeAtlas.unbind();
    }

    _distanceShaderProgram.stopUsing();
  }
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderSpotLightShadowMaps() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderSpotLightShadowMaps");

  glEnable(GL_DEPTH_TEST);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraComponent = RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>());

  std::unordered_set<Entity> spotLightEntities;
  for (const auto& [entity, spotLightComponent] : RegistryViewer::readAll<SpotLightComponent>()) {
    spotLightEntities.insert(entity);
    if (_spotLightToShadowMap.find(entity) == _spotLightToShadowMap.end()) {
      _spotLightToShadowMap.emplace(entity, DepthAtlasFrameBuffer(1));
      auto& newShadowMap = _spotLightToShadowMap.at(entity);

      newShadowMap.bind();
      newShadowMap.setSize(1500, 1500);
      newShadowMap.clear();
      newShadowMap.unbind();
    }
  }

  std::vector<Entity> entitiesWithFrameBuffersToRemove;
  for (const auto& [entity, _buffer] : _spotLightToShadowMap) {
    if (spotLightEntities.find(entity) == spotLightEntities.end()) {
      entitiesWithFrameBuffersToRemove.push_back(entity);
    }
  }

  for (const auto& entityToRemove : entitiesWithFrameBuffersToRemove) {
    _spotLightToShadowMap.erase(entityToRemove);
  }

  for (auto& [lightEntity, shadowMap] : _spotLightToShadowMap) {
    shadowMap.bind();
    shadowMap.clear();
    shadowMap.unbind();

    const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(lightEntity, _registry);
    const auto lightPosition = glm::vec3{lightTransformMatrix[3]};
    const auto& lightRange = RegistryViewer::read<SpotLightComponent>(lightEntity).range;
    const auto& lightOuterCutoff = RegistryViewer::read<SpotLightComponent>(lightEntity).outerCutoff;
    const auto lightSpaceMatrix =
        RegistryViewer::read<SpotLightComponent>(lightEntity).getLightSpaceMatrix(lightTransformMatrix);

    _distanceShaderProgram.use();

    shadowMap.bind(0);
    _distanceShaderProgram.setUniform("projectionViewMatrix", lightSpaceMatrix);
    for (const auto& [modelEntity, modelComponent] : RegistryViewer::readAll<ModelComponent>()) {
      const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
      _distanceShaderProgram.setUniform("modelMatrix", transformMatrix);
      modelComponent.updateImpl();
    }
    shadowMap.unbind();

    _distanceShaderProgram.stopUsing();
  }
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderWorldObjects() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderWorldObjects");

  glEnable(GL_DEPTH_TEST);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraTransformMatrix =
      TransformComponent::getGlobalTransformMatrix(*_registry->getEntity<MainCameraTag>(), _registry);
  const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};
  const auto& cameraComponent = RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>());

  // ********* Rendering the world *********
  UseShader modelShaderUsage(_modelShaderProgram);
  _mostRecentFrameBufferPtr->bind();

  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();

  _modelShaderProgram.setUniform("projectionViewMatrix", projectionMatrix * viewMatrix);
  _modelShaderProgram.setUniform("viewPos", cameraPosition);

  glm::vec3 ambientColor{0, 0, 0};
  for (const auto& ambientLightComponent : RegistryViewer::read<AmbientLightComponent>()) {
    ambientColor += ambientLightComponent.color * ambientLightComponent.intensity;
  }
  _modelShaderProgram.setUniform("ambientLight", ambientColor);

  int pointLightIndex = 0;
  for (const auto& [entity, pointLightComponent] : RegistryViewer::readAll<PointLightComponent>()) {
    auto& depthCubeMap = _pointLightToShadowMap.at(entity);
    depthCubeMap.getTexture().activate(GL_TEXTURE0 + _textureIndex);
    _modelShaderProgram.setUniform("pointLightsDepthCubeMap[" + std::to_string(pointLightIndex) + "]", _textureIndex);
    ++_textureIndex;

    const auto& transformComponent = RegistryViewer::read<TransformComponent>(entity);
    _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].position",
                                   transformComponent.getPosition());
    _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].color",
                                   pointLightComponent.intensity * pointLightComponent.color);
    _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].range",
                                   pointLightComponent.range);
    ++pointLightIndex;
  }
  _modelShaderProgram.setUniform("numPointLights", pointLightIndex);

  int directionalLightIndex = 0;
  for (const auto& [entity, directionalLightComponent] : RegistryViewer::readAll<DirectionalLightComponent>()) {
    const auto& lightTransform = RegistryViewer::read<TransformComponent>(entity);
    const auto& depthAtlasFrameBuffer = _directionalLightToShadowMap.at(entity);
    const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);

    for (size_t levelOfDetail = 0; levelOfDetail < DirectionalLightComponent::LevelsOfDetail; levelOfDetail++) {
      const glm::mat4 lightSpaceMatrix = directionalLightComponent.getLightSpaceMatrix(
          lightTransformMatrix, cameraTransform, cameraComponent, levelOfDetail);
      _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) +
                                         "].lightSpaceMatrix[" + std::to_string(levelOfDetail) + "]",
                                     lightSpaceMatrix);
    }

    depthAtlasFrameBuffer.getTexture().activate(GL_TEXTURE0 + _textureIndex);
    _modelShaderProgram.setUniform("directionalLightsDepthMap[" + std::to_string(directionalLightIndex) + "]",
                                   _textureIndex);
    ++_textureIndex;

    glm::vec3 lightDirection = DirectionalLightComponent::getDirection(lightTransformMatrix);
    _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].direction",
                                   lightDirection);
    _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].color",
                                   directionalLightComponent.intensity * directionalLightComponent.color);
    ++directionalLightIndex;
  }
  _modelShaderProgram.setUniform("numDirectionalLights", directionalLightIndex);

  int spotLightIndex = 0;
  for (const auto& [entity, spotLightComponent] : RegistryViewer::readAll<SpotLightComponent>()) {
    {
      const auto& lightTransform = RegistryViewer::read<TransformComponent>(entity);
      const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
      const auto globalLightPosition = glm::vec3{lightTransformMatrix[3]};
      const auto lightDirection = glm::normalize(glm::vec3{1, 0, 0} * glm::mat3{glm::inverse(lightTransformMatrix)});
      const auto lightSpaceMatrix =
          RegistryViewer::read<SpotLightComponent>(entity).getLightSpaceMatrix(lightTransformMatrix);

      _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].lightSpaceMatrix",
                                     lightSpaceMatrix);
      _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].position",
                                     globalLightPosition);
      _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].direction", lightDirection);
      _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].color",
                                     spotLightComponent.intensity * spotLightComponent.color);
      _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].range",
                                     spotLightComponent.range);
      _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].innerCutoffCosine",
                                     std::cos(spotLightComponent.innerCutoff));
      _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].outerCutoffCosine",
                                     std::cos(spotLightComponent.outerCutoff));
    }

    {
      const auto& depthBuffer = _spotLightToShadowMap.at(entity);
      depthBuffer.getTexture().activate(GL_TEXTURE0 + _textureIndex);
      _modelShaderProgram.setUniform("spotLightsShadowMap[" + std::to_string(spotLightIndex) + "]", _textureIndex);
      ++_textureIndex;
    }

    ++spotLightIndex;
  }
  _modelShaderProgram.setUniform("numSpotLights", spotLightIndex);

  {  // SSAO
    _ssaoFrameBuffer.getTexture().activate(GL_TEXTURE0 + _textureIndex);
    _modelShaderProgram.setUniform("ssao", _textureIndex);
    ++_textureIndex;

    _modelShaderProgram.setUniform("viewportSize", _mostRecentFrameBufferPtr->getSize());
  }

  {
    SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderWorldObjects - Models");
    for (const auto& [entity, modelComponent] : RegistryViewer::readAll<ModelComponent>()) {

      const auto& transformMatrix = utils::transform::getGlobalTransformMatrix(entity, *this);
      _modelShaderProgram.setUniform("modelMatrix", transformMatrix);

      const auto& material = std::get<AssetRef<Material>>(modelComponent.material);
      setMaterial(material.get());

      modelComponent.updateImpl();
    }
  }
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderParticles() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderParticles");

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);

  UseShader modelShaderUsage(_modelShaderProgram);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraTransformMatrix =
      TransformComponent::getGlobalTransformMatrix(*_registry->getEntity<MainCameraTag>(), _registry);
  const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);

  static const auto billboardPlane = utils::assets::readPermanent<Mesh>(Mesh::Factory::Type::PLANE);
  billboardPlane->bind();

  for (const auto& [entity, emitter] : RegistryViewer::readAll<ParticleEmitterComponent>()) {
    if (!emitter.enabled) {
      continue;
    }

    auto cameraFacingTransform = glm::mat4{1.0};
    cameraFacingTransform =
        utils::transform::getCameraFacingModelMatrix(cameraFacingTransform, cameraPosition, viewMatrix);

    const auto& material = std::get<AssetRef<Material>>(emitter.material);
    setMaterial(material.get());

    const auto& state = emitter.state;

    for (size_t i = 0; i < state.lifetimes.size(); ++i) {
      if (state.lifetimes[i] < 0) {
        continue;
      }

      const auto particleScale = state.sizes[i];
      auto cameraFacingTransformOfParticle = glm::scale(cameraFacingTransform, glm::vec3{state.sizes[i]});
      cameraFacingTransformOfParticle[3] = glm::vec4{state.positions[i], 1.0f};
      _modelShaderProgram.setUniform("modelMatrix", cameraFacingTransformOfParticle);
      _modelShaderProgram.setUniform("material.alphaMultiplier", state.transparencies[i]);

      glDrawElements(GL_TRIANGLES, billboardPlane->getMeshSize(), GL_UNSIGNED_INT, nullptr);
    }
  }

  billboardPlane->unbind();

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderBillboards() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderBillboards");

  UseShader modelShaderUsage(_modelShaderProgram);
  _mostRecentFrameBufferPtr->bind();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraTransformMatrix =
      TransformComponent::getGlobalTransformMatrix(*_registry->getEntity<MainCameraTag>(), _registry);
  const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);

  static const auto billboardPlane = utils::assets::readPermanent<Mesh>(Mesh::Factory::Type::PLANE);

  billboardPlane->bind();
  for (const auto& [entity, billboardComponent] :
       RegistryViewer::readAll<BillboardComponent<RuntimeMode::PRODUCTION>>()) {
    if (billboardComponent.occlusion == BillboardComponent<RuntimeMode::PRODUCTION>::Occlusion::Occludable) {
      glEnable(GL_DEPTH_TEST);
    } else {
      glDisable(GL_DEPTH_TEST);
    }

    const auto& baseTransform = TransformComponent::getGlobalTransformMatrix(entity, _registry);
    glm::mat4 modelMatrix = billboardComponent.getModelMatrix(baseTransform, cameraPosition, viewMatrix);
    _modelShaderProgram.setUniform("modelMatrix", modelMatrix);

    const auto& material = std::get<AssetRef<Material>>(billboardComponent.material);
    setMaterial(material.get());

    glDrawElements(GL_TRIANGLES, billboardPlane->getMeshSize(), GL_UNSIGNED_INT, nullptr);
  }
  billboardPlane->unbind();

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

  _mostRecentFrameBufferPtr->unbind();
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderDebugBillboards() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderBillboards");

  UseShader modelShaderUsage(_modelShaderProgram);
  _mostRecentFrameBufferPtr->bind();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraTransformMatrix =
      TransformComponent::getGlobalTransformMatrix(*_registry->getEntity<MainCameraTag>(), _registry);
  const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);

  static const auto billboardPlane = utils::assets::readPermanent<Mesh>(Mesh::Factory::Type::PLANE);
  billboardPlane->bind();

  const auto drawDebugBillboard = [this, &cameraPosition, &viewMatrix](const auto& componentSet,
                                                                       Material const* material) {
    static auto debugBillboard = BillboardComponent<>{.orientation = BillboardComponent<>::Orientation::CameraFacing,
                                                      .scale = BillboardComponent<>::Scale::Camera,
                                                      .occlusion = BillboardComponent<>::Occlusion::NotOccludable};

    for (const auto& [entity, _] : componentSet) {
      auto baseTransform = TransformComponent::getGlobalTransformMatrix(entity, _registry);

      glm::vec3 translation = glm::vec3(baseTransform[3]);              // Extract translation
      glm::vec3 right = glm::normalize(glm::vec3(baseTransform[0]));    // X-axis
      glm::vec3 up = glm::normalize(glm::vec3(baseTransform[1]));       // Y-axis
      glm::vec3 forward = glm::normalize(glm::vec3(baseTransform[2]));  // Z-axis

      baseTransform = glm::mat4(1.0f);
      baseTransform[0] = glm::vec4(right, 0.0f);
      baseTransform[1] = glm::vec4(up, 0.0f);
      baseTransform[2] = glm::vec4(forward, 0.0f);
      baseTransform[3] = glm::vec4(translation, 1.0f);

      glm::mat4 modelMatrix = debugBillboard.getModelMatrix(baseTransform, cameraPosition, viewMatrix);

      _modelShaderProgram.setUniform("modelMatrix", modelMatrix);

      setMaterial(material);

      // Draw the billboard quad.
      glDrawElements(GL_TRIANGLES, billboardPlane->getMeshSize(), GL_UNSIGNED_INT, nullptr);
    }
  };

  {
    drawDebugBillboard(RegistryViewer::readAll<PointLightComponent>(), &_pointLightDebugMaterial);
    drawDebugBillboard(RegistryViewer::readAll<DirectionalLightComponent>(), &_directionalLightDebugMaterial);
    drawDebugBillboard(RegistryViewer::readAll<SpotLightComponent>(), &_spotLightDebugMaterial);
    drawDebugBillboard(RegistryViewer::readAll<AmbientLightComponent>(), &_ambientLightDebugMaterial);
    drawDebugBillboard(RegistryViewer::readAll<ParticleEmitterComponent>(), &_particleEmitterDebugMaterial);
    drawDebugBillboard(RegistryViewer::readAll<PostProcessingVolumeComponent>(), &_postProcessingVolumeDebugMaterial);
  }

  billboardPlane->unbind();

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  _mostRecentFrameBufferPtr->unbind();
}
template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderPostProcessingVolumes() {
  glEnable(GL_DEPTH_TEST);

  UseShader modelShaderUsage(_wireframeShaderProgram);
  _mostRecentFrameBufferPtr->bind();

  static const glm::vec3 volumeWireframeColor{0.8, 0.8, 0.8};
  _wireframeShaderProgram.setUniform("fixedColor", volumeWireframeColor);

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();
  const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

  static const auto volumeCube = utils::assets::readPermanent<Wireframe>(Wireframe::Factory::Type::CUBE);

  volumeCube->bind();
  for (const auto& [entity, _volumeComponent] : RegistryViewer::readAll<PostProcessingVolumeComponent>()) {
    const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
    _wireframeShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
    glDrawArrays(GL_LINES, 0, volumeCube->getEdgeCount());
  }
  volumeCube->unbind();

  _mostRecentFrameBufferPtr->unbind();
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::bloom(float threshold, float weight) {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderBloom");

  glDisable(GL_DEPTH_TEST);

  // Downscaling Pass
  utils::applyShaderToFrameBuffer(_downscaledFrameBuffers[0], _thresholdShaderProgram,
                                  {{"sceneTexture", &_mostRecentFrameBufferPtr->getTexture()}},
                                  utils::Uniform("threshold", threshold));
  for (size_t i = 1; i < BloomSteps; ++i) {
    utils::applyShaderToFrameBuffer(_downscaledFrameBuffers[i], _thresholdShaderProgram,
                                    {{"sceneTexture", &_downscaledFrameBuffers[i - 1].getTexture()}},
                                    utils::Uniform("threshold", 0.0f));
  }

  // Horizontal Blur Pass
  for (size_t i = 0; i < BloomSteps; ++i) {
    utils::applyShaderToFrameBuffer(_horizontallyBluredDownscaledFrameBuffers[i], _horizontalBlurShaderProgram,
                                    {{"textureToBlur", &_downscaledFrameBuffers[i].getTexture()}},
                                    utils::Uniform("resolution", _downscaledFrameBuffers[i].getSize()));
  }

  // Vertical Blur Pass
  for (size_t i = 0; i < BloomSteps; ++i) {
    utils::applyShaderToFrameBuffer(
        _fullyBluredDownscaledFrameBuffers[i], _verticalBlurShaderProgram,
        {{"textureToBlur", &_horizontallyBluredDownscaledFrameBuffers[i].getTexture()}},
        utils::Uniform("resolution", _horizontallyBluredDownscaledFrameBuffers[i].getSize()));
  }

  // Blur Combination Pass
  auto* currentBuffer = &_fullyBluredDownscaledFrameBuffers[BloomSteps - 1];
  float currentWeight = 1.0f / (1 << (BloomSteps - 1));

  for (int i = BloomSteps - 2; i >= 0; --i) {
    auto* nextBuffer = &_fullyBluredDownscaledFrameBuffers[i];
    float nextWeight = 1.0f / (1 << i);

    utils::applyShaderToFrameBuffer(
        *currentBuffer, _weightedAdditionShaderProgram,
        {{"firstTexture", &currentBuffer->getTexture()}, {"secondTexture", &nextBuffer->getTexture()}},
        utils::Uniform("firstWeight", currentWeight), utils::Uniform("secondWeight", nextWeight));

    currentWeight += nextWeight;
  }

  // Composite bloom with the original scene
  utils::applyShaderToFrameBuffer(
      _bloomedFrameBuffer, _weightedAdditionShaderProgram,
      {{"firstTexture", &_mostRecentFrameBufferPtr->getTexture()}, {"secondTexture", &currentBuffer->getTexture()}},
      utils::Uniform("firstWeight", 1.0f), utils::Uniform("secondWeight", weight));

  glEnable(GL_DEPTH_TEST);
  _mostRecentFrameBufferPtr = &_bloomedFrameBuffer;
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::toneMapping() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::toneMapping");

  glDisable(GL_DEPTH_TEST);

  utils::applyShaderToFrameBuffer(_toneMappedFrameBuffer, _toneMappingShaderProgram,
                                  {{"original", &_mostRecentFrameBufferPtr->getTexture()}});

  glEnable(GL_DEPTH_TEST);
  _mostRecentFrameBufferPtr = &_toneMappedFrameBuffer;
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::gammaCorrection(float gamma) {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::gammaCorrection");

  glDisable(GL_DEPTH_TEST);

  utils::applyShaderToFrameBuffer(_gammaCorrectedFrameBuffer, _gammaCorrectionShaderProgram,
                                  {{"original", &_mostRecentFrameBufferPtr->getTexture()}},
                                  utils::Uniform("gamma", gamma));

  glEnable(GL_DEPTH_TEST);
  _mostRecentFrameBufferPtr = &_gammaCorrectedFrameBuffer;
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderWireframes() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderWireframes");

  glEnable(GL_DEPTH_TEST);

  _mostRecentFrameBufferPtr->bind();

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();

  _wireframeShaderProgram.use();

  const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

  static const glm::vec3 wireframeColor{0.08, 0.7, 0.15};

  for (const auto& [entity, wireframeComponent] : RegistryViewer::readAll<WireframeComponent>()) {
    const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
    _wireframeShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
    _wireframeShaderProgram.setUniform("fixedColor", wireframeColor);
    wireframeComponent.updateImpl();
  }

  _wireframeShaderProgram.stopUsing();
  _mostRecentFrameBufferPtr->unbind();
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderSkybox() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderSkybox");

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();
  static auto cubemapMesh = utils::assets::readPermanent<Mesh>(Mesh::Factory::Type::CUBEMAP);

  glDepthFunc(GL_LEQUAL);

  _mostRecentFrameBufferPtr->bind();
  _skyboxShaderProgram.use();
  _skyboxShaderProgram.setUniform("viewMatrix", glm::mat4(viewMatrix));
  _skyboxShaderProgram.setUniform("projectionMatrix", projectionMatrix);

  for (const auto& skybox : RegistryViewer::read<SkyboxComponent>()) {
    const auto& skyboxCubeMap = std::get<AssetRef<CubeMap>>(skybox.skyboxCubeMap);
    SHKYERA_ASSERT(skyboxCubeMap, "Skybox CubeMap Asset is not loaded");

    skyboxCubeMap->activate(GL_TEXTURE0);
    _skyboxShaderProgram.setUniform("skybox", 0);
    cubemapMesh->draw();
  }

  _skyboxShaderProgram.stopUsing();
  _mostRecentFrameBufferPtr->unbind();

  glDepthFunc(GL_LESS);
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::renderOverlayModels() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::renderOverlayModels");

  glDisable(GL_DEPTH_TEST);

  _mostRecentFrameBufferPtr->bind();

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const glm::mat4& viewMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getViewMatrix(cameraTransform);
  const glm::mat4& projectionMatrix =
      RegistryViewer::read<CameraComponent>(*_registry->getEntity<MainCameraTag>()).getProjectionMatrix();

  _wireframeShaderProgram.use();

  const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

  for (const auto& [entity, overlayModelComponent] : RegistryViewer::readAll<OverlayModelComponent>()) {
    const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
    _wireframeShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
    _wireframeShaderProgram.setUniform("fixedColor", overlayModelComponent.getMaterial()->albedo);
    overlayModelComponent.updateImpl();
  }

  _wireframeShaderProgram.stopUsing();
  _mostRecentFrameBufferPtr->unbind();

  glEnable(GL_DEPTH_TEST);
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::antiAliasing() {
  SHKYERA_PROFILE("RenderingSystem<MainCameraTag>::antiAliasing");
  glDisable(GL_DEPTH_TEST);
  utils::applyShaderToFrameBuffer(_antiAliasedFrameBuffer, _antiAliasingShaderProgram,
                                  {{"sceneTexture", &_mostRecentFrameBufferPtr->getTexture()}});

  glEnable(GL_DEPTH_TEST);
  _mostRecentFrameBufferPtr = &_antiAliasedFrameBuffer;
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::setTexture(Material const* material, const std::string& textureName,
                                                auto textureMember) {
  UseShader modelShaderUsage(_modelShaderProgram);
  if (const auto& textureAsset = std::get<AssetRef<Texture>>((*material).*textureMember)) {
    _modelShaderProgram.setUniform("material." + textureName + "TextureLoaded", true);

    textureAsset->activate(GL_TEXTURE0 + _textureIndex);
    _modelShaderProgram.setUniform("material." + textureName + "Texture", _textureIndex);
    ++_textureIndex;
  } else {
    _modelShaderProgram.setUniform("material." + textureName + "TextureLoaded", false);
  }
}

template <typename MainCameraTag>
void RenderingSystem<MainCameraTag>::setMaterial(Material const* material) {
  UseShader modelShaderUsage(_modelShaderProgram);
  if (material == nullptr) {
    material = &_defaultMaterial;
  }

  _modelShaderProgram.setUniform("material.lit", material->lit);
  _modelShaderProgram.setUniform("material.alphaMultiplier", material->alphaMultiplier);
  _modelShaderProgram.setUniform("material.albedoColor", material->albedo);
  _modelShaderProgram.setUniform("material.emissive", material->emissive * material->emissiveStrength);
  _modelShaderProgram.setUniform("material.roughness", material->roughness);
  _modelShaderProgram.setUniform("material.metallic", material->metallic);
  _modelShaderProgram.setUniform("material.normalMapStrength", material->normalMapStrength);

  setTexture(material, "roughness", &Material::roughnessTexture);
  setTexture(material, "albedo", &Material::albedoTexture);
  setTexture(material, "normal", &Material::normalTexture);
  setTexture(material, "metallic", &Material::metallicTexture);
  setTexture(material, "emissive", &Material::emissiveTexture);
}

template <typename MainCameraTag>
PostProcessingVolumeComponent RenderingSystem<MainCameraTag>::getPostProcessingSettings() {
  static const auto volume = AABB{.center = {0, 0, 0}, .extents = {1.0, 1.0, 1.0}};

  const auto& cameraTransform = RegistryViewer::read<TransformComponent>(*_registry->getEntity<MainCameraTag>());
  const auto& cameraTransformMatrix =
      TransformComponent::getGlobalTransformMatrix(*_registry->getEntity<MainCameraTag>(), _registry);
  const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};

  for (const auto& [entity, postProcessingVolume] : RegistryViewer::readAll<PostProcessingVolumeComponent>()) {
    const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
    if (volume.isInside(transformMatrix, cameraPosition)) {
      return postProcessingVolume;
    }
  }

  for (const auto& postProcessingVolume : RegistryViewer::read<PostProcessingVolumeComponent>()) {
    if (postProcessingVolume.global) {
      return postProcessingVolume;
    }
  }

  return PostProcessingVolumeComponent{};
}

}  // namespace shkyera
