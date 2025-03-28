#include <chrono>
#include <random>

#include <Systems/RenderingSystem.hpp>
#include <Utils/AssetUtils.hpp>
#include <Utils/TransformUtils.hpp>
#include <Math/AABB.hpp>
#include <Common/Logger.hpp>
#include <Common/Random.hpp>
#include <Common/Profiler.hpp>
#include <Rendering/Utils.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/BillboardComponent.hpp>
#include <Components/OverlayModelComponent.hpp>
#include <Components/PostProcessingVolumeComponent.hpp>
#include <Components/ParticleEmitterComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/AmbientLightComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <Components/SpotLightComponent.hpp>
#include <Components/DirectionalLightComponent.hpp>

namespace shkyera {

RenderingSystem::RenderingSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    const auto& positionAndNormalVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/position_and_normal.glsl", Shader::Type::Vertex);
    const auto& modelFragmentShader =  utils::assets::readPermanent<Shader>("resources/shaders/fragment/uber.glsl", Shader::Type::Fragment);
    _modelShaderProgram.attachShader(positionAndNormalVertexShader);
    _modelShaderProgram.attachShader(modelFragmentShader);
    _modelShaderProgram.link();

    const auto& distanceVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/distance.glsl", Shader::Type::Vertex);
    const auto& distanceFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/distance.glsl", Shader::Type::Fragment);
    _distanceShaderProgram.attachShader(distanceVertexShader);
    _distanceShaderProgram.attachShader(distanceFragmentShader);
    _distanceShaderProgram.link();

    const auto& viewSpaceNormalVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/viewspace_normal.glsl", Shader::Type::Vertex);
    const auto& viewSpaceNormalFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/viewspace_normal.glsl", Shader::Type::Fragment);
    _viewSpaceNormalShaderProgram.attachShader(viewSpaceNormalVertexShader);
    _viewSpaceNormalShaderProgram.attachShader(viewSpaceNormalFragmentShader);
    _viewSpaceNormalShaderProgram.link();

    const auto& viewSpaceDepthVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/viewspace_position.glsl", Shader::Type::Vertex);
    const auto& viewSpaceDepthFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/viewspace_position.glsl", Shader::Type::Fragment);
    _viewSpacePositionShaderProgram.attachShader(viewSpaceDepthVertexShader);
    _viewSpacePositionShaderProgram.attachShader(viewSpaceDepthFragmentShader);
    _viewSpacePositionShaderProgram.link();

    const auto& texCoordsVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/texcoords.glsl", Shader::Type::Vertex);
    const auto& ssaoFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/ssao.glsl", Shader::Type::Fragment);
    _ssaoShaderProgram.attachShader(texCoordsVertexShader);
    _ssaoShaderProgram.attachShader(ssaoFragmentShader);
    _ssaoShaderProgram.link();

    const auto& toneMappingFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/tonemapping_aces.glsl", Shader::Type::Fragment);
    _toneMappingShaderProgram.attachShader(texCoordsVertexShader);
    _toneMappingShaderProgram.attachShader(toneMappingFragmentShader);
    _toneMappingShaderProgram.link();

    const auto& gammaCorrectionFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/gamma_correction.glsl", Shader::Type::Fragment);
    _gammaCorrectionShaderProgram.attachShader(texCoordsVertexShader);
    _gammaCorrectionShaderProgram.attachShader(gammaCorrectionFragmentShader);
    _gammaCorrectionShaderProgram.link();

    const auto& luminosityThresholdFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/luminosity_threshold.glsl", Shader::Type::Fragment);
    _thresholdShaderProgram.attachShader(texCoordsVertexShader);
    _thresholdShaderProgram.attachShader(luminosityThresholdFragmentShader);
    _thresholdShaderProgram.link();

    const auto& horizontalGaussianBlur5FragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/horizontal_gaussianblur_5.glsl", Shader::Type::Fragment);
    _horizontalBlurShaderProgram.attachShader(texCoordsVertexShader);
    _horizontalBlurShaderProgram.attachShader(horizontalGaussianBlur5FragmentShader);
    _horizontalBlurShaderProgram.link();

    const auto& verticalGaussianBlur5FragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/vertical_gaussianblur_5.glsl", Shader::Type::Fragment);
    _verticalBlurShaderProgram.attachShader(texCoordsVertexShader);
    _verticalBlurShaderProgram.attachShader(verticalGaussianBlur5FragmentShader);
    _verticalBlurShaderProgram.link();

    const auto& weightedAdditionFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/weighted_addition.glsl", Shader::Type::Fragment);
    _weightedAdditionShaderProgram.attachShader(texCoordsVertexShader);
    _weightedAdditionShaderProgram.attachShader(weightedAdditionFragmentShader);
    _weightedAdditionShaderProgram.link();

    const auto& positionVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/position.glsl", Shader::Type::Vertex);
    const auto& fixedColorFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/color.glsl", Shader::Type::Fragment);
    _wireframeShaderProgram.attachShader(positionVertexShader);
    _wireframeShaderProgram.attachShader(fixedColorFragmentShader);
    _wireframeShaderProgram.link();

    _silhouetteShaderProgram.attachShader(positionVertexShader);
    _silhouetteShaderProgram.attachShader(fixedColorFragmentShader);
    _silhouetteShaderProgram.link();

    const auto& dilateFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/dilate.glsl", Shader::Type::Fragment);
    _dilateShaderProgram.attachShader(texCoordsVertexShader);
    _dilateShaderProgram.attachShader(dilateFragmentShader);
    _dilateShaderProgram.link();

    const auto& subtractFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/subtract.glsl", Shader::Type::Fragment);
    _subtractShaderProgram.attachShader(texCoordsVertexShader);
    _subtractShaderProgram.attachShader(subtractFragmentShader);
    _subtractShaderProgram.link();

    const auto& overlayFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/overlay.glsl", Shader::Type::Fragment);
    _overlayShaderProgram.attachShader(texCoordsVertexShader);
    _overlayShaderProgram.attachShader(overlayFragmentShader);
    _overlayShaderProgram.link();

    const auto& fxaaFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/fxaa.glsl", Shader::Type::Fragment);
    _antiAliasingShaderProgram.attachShader(texCoordsVertexShader);
    _antiAliasingShaderProgram.attachShader(fxaaFragmentShader);
    _antiAliasingShaderProgram.link();

    const auto& skyboxVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/skybox.glsl", Shader::Type::Vertex);
    const auto& cubeMapFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/cubemap.glsl", Shader::Type::Fragment);
    _skyboxShaderProgram.attachShader(skyboxVertexShader);
    _skyboxShaderProgram.attachShader(cubeMapFragmentShader);
    _skyboxShaderProgram.link();

    const auto& depthVertexShader = utils::assets::readPermanent<Shader>("resources/shaders/vertex/depth.glsl", Shader::Type::Vertex);
    const auto& depthFragmentShader = utils::assets::readPermanent<Shader>("resources/shaders/fragment/depth.glsl", Shader::Type::Fragment);
    _depthShaderProgram.attachShader(depthVertexShader);
    _depthShaderProgram.attachShader(depthFragmentShader);
    _depthShaderProgram.link();

    _defaultMaterial.lit = false;
    _defaultMaterial.albedo = glm::vec3{1, 0, 1};

    Material debugBillboardMaterial;
    debugBillboardMaterial.lit = false;
    _particleEmitterDebugMaterial 
        = _directionalLightDebugMaterial
        = _spotLightDebugMaterial 
        = _pointLightDebugMaterial 
        = _ambientLightDebugMaterial 
        = _postProcessingVolumeDebugMaterial 
        = debugBillboardMaterial;

    std::get<AssetRef<Texture>>(_pointLightDebugMaterial.albedoTexture) = utils::assets::readPermanent<Texture>("resources/icons/components/point_light.png");
    std::get<AssetRef<Texture>>(_spotLightDebugMaterial.albedoTexture) = utils::assets::readPermanent<Texture>("resources/icons/components/spot_light.png");
    std::get<AssetRef<Texture>>(_directionalLightDebugMaterial.albedoTexture) = utils::assets::readPermanent<Texture>("resources/icons/components/directional_light.png");
    std::get<AssetRef<Texture>>(_ambientLightDebugMaterial.albedoTexture) = utils::assets::readPermanent<Texture>("resources/icons/components/ambient_light.png");
    std::get<AssetRef<Texture>>(_particleEmitterDebugMaterial.albedoTexture) = utils::assets::readPermanent<Texture>("resources/icons/components/particles_gray.png");
    std::get<AssetRef<Texture>>(_postProcessingVolumeDebugMaterial.albedoTexture) = utils::assets::readPermanent<Texture>("resources/icons/components/post_processing_gray.png");
}

void RenderingSystem::setSize(uint32_t width, uint32_t height)
{
    SHKYERA_PROFILE("RenderingSystem::setSize");

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
    for(size_t i = 0; i < BloomSteps; ++i)
    {
        _downscaledFrameBuffers[i].setSize(width / downscaleFactor, height / downscaleFactor);
        _horizontallyBluredDownscaledFrameBuffers[i].setSize(width / downscaleFactor, height / downscaleFactor);
        _fullyBluredDownscaledFrameBuffers[i].setSize(width / downscaleFactor, height / downscaleFactor);
        downscaleFactor *= 1.5;
    }
}

GLuint RenderingSystem::getRenderFrameBuffer()
{
    return _mostRecentFrameBufferPtr->getTexture().getID();
}

void RenderingSystem::clearFrameBuffers()
{
    SHKYERA_PROFILE("RenderingSystem::clearFrameBuffers");

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

    for(auto& buffer : _downscaledFrameBuffers)
    {
        buffer.clear();
    }
    for(auto& buffer : _horizontallyBluredDownscaledFrameBuffers)
    {
        buffer.clear();
    }
    for(auto& buffer : _fullyBluredDownscaledFrameBuffers)
    {
        buffer.clear();
    }
}

void RenderingSystem::render() 
{   
    _textureIndex = 0;
    SHKYERA_PROFILE("RenderingSystem::render");

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
    renderWorldObjects(postProcessing);
    renderParticles();
    renderBillboards();
    renderPostProcessingVolumes();
    renderWireframes();
    renderOutline(_registry->getSelectedEntities());

    // Post-Processing
    if(postProcessing.bloomWeight > 0.0f) bloom(postProcessing.bloomThreshold, postProcessing.bloomWeight);
    if(postProcessing.toneMapping) toneMapping();
    if(postProcessing.gamma != 1.0f) gammaCorrection(postProcessing.gamma);
    if(postProcessing.antiAliasing) antiAliasing();

    renderOverlayModels();
}

void RenderingSystem::renderViewPosition()
{
    SHKYERA_PROFILE("RenderingSystem::renderViewPosition");

    _viewSpacePositionShaderProgram.use();
    _viewSpacePositionFrameBuffer.bind();

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _viewSpacePositionShaderProgram.setUniform("projection", projectionMatrix);
    _viewSpacePositionShaderProgram.setUniform("view", viewMatrix);

    for (const auto& [entity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        _viewSpacePositionShaderProgram.setUniform("model", transformMatrix);

        modelComponent.updateImpl();
    }

    _viewSpacePositionFrameBuffer.unbind();
    _viewSpacePositionShaderProgram.stopUsing();
}

void RenderingSystem::renderViewNormals()
{
    SHKYERA_PROFILE("RenderingSystem::renderViewNormals");

    _viewSpaceNormalShaderProgram.use();
    _viewSpaceNormalFrameBuffer.bind();

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _viewSpaceNormalShaderProgram.setUniform("projection", projectionMatrix);
    _viewSpaceNormalShaderProgram.setUniform("view", viewMatrix);

    for (const auto& [entity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        _viewSpaceNormalShaderProgram.setUniform("model", transformMatrix);

        modelComponent.updateImpl();
    }

    _viewSpaceNormalFrameBuffer.unbind();
    _viewSpaceNormalShaderProgram.stopUsing();
}

void RenderingSystem::renderSSAO(float strength, float radius)
{
    SHKYERA_PROFILE("RenderingSystem::renderSSAO");

    static std::vector<glm::vec3> ssaoKernel;
    static float usedRadius;

    bool updatedKernel = false;
    if(ssaoKernel.empty() || radius != usedRadius)
    {
        constexpr int SSAOSamples = 128;
        updatedKernel = true;
        usedRadius = radius;

        ssaoKernel.resize(SSAOSamples);
        random::Uniform<float> sampler(0.0f, 1.0f);
        for (int i = 0; i < SSAOSamples; ++i) {
            glm::vec3 sample(
                sampler() * 2.0 - 1.0,
                sampler() * 2.0 - 1.0,
                sampler() * 0.95 + 0.05
            );
            sample = glm::normalize(sample);

            float scale = (float)i / SSAOSamples;
            scale = glm::mix(0.05f, usedRadius, scale * scale);
            sample *= scale;

            ssaoKernel[i] = sample;
        }
    }

    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();
    
    UseShader ssaoShaderUsage(_ssaoShaderProgram);

    _ssaoShaderProgram.setUniform("strength", strength);

    _viewSpacePositionFrameBuffer.getTexture().activate(GL_TEXTURE0);
    _ssaoShaderProgram.setUniform("depthTexture", 0);
    _viewSpaceNormalFrameBuffer.getTexture().activate(GL_TEXTURE1);
    _ssaoShaderProgram.setUniform("normalTexture", 1);

    if(updatedKernel)
    {
        for(int i = 0; i < 128; i++)
        {
            _ssaoShaderProgram.setUniform("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
        }
    }
    
    // Set uniforms using the parameter pack
    _ssaoShaderProgram.setUniform("projection", projectionMatrix);

    // Draw fullscreen quad
    _ssaoFrameBuffer.bind();
    utils::drawFullscreenQuad();
    _ssaoFrameBuffer.unbind();
}

void RenderingSystem::renderOutline(const std::unordered_set<Entity>& entities)
{
    if(std::none_of(entities.begin(), entities.end(), [this](auto e) { return _registry->hasComponent<NameComponent>(e); }))
    {
        return;
    }

    SHKYERA_PROFILE("RenderingSystem::renderOutline");

    for(const auto& entity : entities)
    {
        const auto& children = _registry->getHierarchy().getChildren(entity);
        renderOutline(children);
    }

    glDisable(GL_DEPTH_TEST);

    // Drawing a silhouette
    _silhouetteFrameBuffer.bind();
    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();
    const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

    _silhouetteShaderProgram.use();
    _silhouetteShaderProgram.setUniform("fixedColor", glm::vec3{1.0, 0.1, 1.0});
    for (const auto& entity : entities) {
        if(_registry->hasComponents<TransformComponent, ModelComponent>(entity))
        {
            const auto& modelComponent = _registry->getComponent<ModelComponent>(entity);
            const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
            _silhouetteShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
            modelComponent.updateImpl();
        }
    }
    _silhouetteShaderProgram.stopUsing();
    _silhouetteFrameBuffer.unbind();

    utils::applyShaderToFrameBuffer(
        _horizontallyDilatedFrameBuffer,
        _dilateShaderProgram,
        {
            { "silhouetteTexture", &_silhouetteFrameBuffer.getTexture() }
        },
        utils::Uniform("horizontal", true),
        utils::Uniform("kernelSize", 3)
    );

    utils::applyShaderToFrameBuffer(
        _fullyDilatedFrameBuffer,
        _dilateShaderProgram,
        {
            { "silhouetteTexture", &_horizontallyDilatedFrameBuffer.getTexture() }
        },
        utils::Uniform("horizontal", false),
        utils::Uniform("kernelSize", 3)
    );

    utils::applyShaderToFrameBuffer(
        _differenceFrameBuffer,
        _subtractShaderProgram,
        {
            { "first", &_fullyDilatedFrameBuffer.getTexture() },
            { "second", &_silhouetteFrameBuffer.getTexture() }
        }
    );

    utils::applyShaderToFrameBuffer(
        _outlinedObjectsFrameBuffer,
        _overlayShaderProgram,
        {
            { "background", &_mostRecentFrameBufferPtr->getTexture() },
            { "overlay", &_differenceFrameBuffer.getTexture() }
        }
    );

    _mostRecentFrameBufferPtr = &_outlinedObjectsFrameBuffer;
}

void RenderingSystem::renderDirectionalLightShadowMaps()
{
    SHKYERA_PROFILE("RenderingSystem::renderDirectionalLightShadowMaps");

    glEnable(GL_DEPTH_TEST);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    std::unordered_set<Entity> directionalLightEntities;
    for(const auto& [entity, directionalLightComponent] : _registry->getComponentSet<DirectionalLightComponent>()) {
        directionalLightEntities.insert(entity);
        if(_directionalLightToShadowMap.find(entity) == _directionalLightToShadowMap.end())
        {
            _directionalLightToShadowMap.emplace(entity, DepthAtlasFrameBuffer(DirectionalLightComponent::LevelsOfDetail));
            auto& newShadowMap = _directionalLightToShadowMap.at(entity);
            
            newShadowMap.bind();
            newShadowMap.setSize(DirectionalLightComponent::LevelsOfDetail * 2048, 2048);
            newShadowMap.clear();
            newShadowMap.unbind();
        }
    }

    std::vector<Entity> entitiesWithFrameBuffersToRemove;
    for(const auto& [entity, _buffer] : _directionalLightToShadowMap)
    {
        if(directionalLightEntities.find(entity) == directionalLightEntities.end())
        {
            entitiesWithFrameBuffersToRemove.push_back(entity);
        }
    }

    for(const auto& entityToRemove : entitiesWithFrameBuffersToRemove)
    {
        _directionalLightToShadowMap.erase(entityToRemove);
    }

    for(auto& [lightEntity, shadowMapAtlas] : _directionalLightToShadowMap)
    {
        shadowMapAtlas.bind();
        shadowMapAtlas.clear();
        shadowMapAtlas.unbind();

        const auto& directionalLightComponent = _registry->getComponent<DirectionalLightComponent>(lightEntity);
        const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(lightEntity, _registry);

        for(uint8_t levelOfDetail = 0; levelOfDetail < DirectionalLightComponent::LevelsOfDetail; ++levelOfDetail)
        {
            shadowMapAtlas.bind(levelOfDetail);
            _depthShaderProgram.use();

            const glm::mat4& lightSpaceMatrix = directionalLightComponent.getLightSpaceMatrix(lightTransformMatrix, cameraTransform, cameraComponent, levelOfDetail);

            _depthShaderProgram.setUniform("projectionViewMatrix", lightSpaceMatrix);
            for (const auto& [modelEntity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
                const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
                _depthShaderProgram.setUniform("modelMatrix", transformMatrix);

                modelComponent.updateImpl();
            }

            _depthShaderProgram.stopUsing();
            shadowMapAtlas.unbind();
        }
    }
}

void RenderingSystem::renderPointLightShadowMaps()
{
    SHKYERA_PROFILE("RenderingSystem::renderPointLightShadowMaps");

    glEnable(GL_DEPTH_TEST);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    std::unordered_set<Entity> pointLightEntities;
    for(const auto& [entity, pointLightComponent] : _registry->getComponentSet<PointLightComponent>()) {
        pointLightEntities.insert(entity);
        if(_pointLightToShadowMap.find(entity) == _pointLightToShadowMap.end())
        {
            _pointLightToShadowMap.emplace(entity, DepthAtlasFrameBuffer(6));
            auto& newShadowMap = _pointLightToShadowMap.at(entity);
            
            newShadowMap.bind();
            newShadowMap.setSize(6 * 1024, 1024);
            newShadowMap.clear();
            newShadowMap.unbind();
        }
    }

    std::vector<Entity> entitiesWithFrameBuffersToRemove;
    for(const auto& [entity, _buffer] : _pointLightToShadowMap)
    {
        if(pointLightEntities.find(entity) == pointLightEntities.end())
        {
            entitiesWithFrameBuffersToRemove.push_back(entity);
        }
    }

    for(const auto& entityToRemove : entitiesWithFrameBuffersToRemove)
    {
        _pointLightToShadowMap.erase(entityToRemove);
    }

    for(auto& [lightEntity, shadowMapCubeAtlas] : _pointLightToShadowMap)
    {
        shadowMapCubeAtlas.bind();
        shadowMapCubeAtlas.clear();
        shadowMapCubeAtlas.unbind();

        // TODO: Use global position
        const auto& lightPosition = _registry->getComponent<TransformComponent>(lightEntity).getPosition();
        const auto& lightRange = _registry->getComponent<PointLightComponent>(lightEntity).range;

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, lightRange);
        glm::mat4 captureViews[6] = {
            projection * glm::lookAt(lightPosition, lightPosition + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f)),  // +X
            projection * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f)),  // -X
            projection * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f,  0.0f,  1.0f)),  // +Y
            projection * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3( 0.0f,  0.0f, -1.0f)),  // -Y
            projection * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f, -1.0f,  0.0f)),  // +Z
            projection * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f, -1.0f,  0.0f))   // -Z
        };

        _distanceShaderProgram.use();

        for(uint8_t face = 0; face < 6; ++face)
        {
            shadowMapCubeAtlas.bind(face);

            const glm::mat4& lightSpaceMatrix = captureViews[face];
            _distanceShaderProgram.setUniform("projectionViewMatrix", lightSpaceMatrix);

            for (const auto& [modelEntity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
                const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
                _distanceShaderProgram.setUniform("modelMatrix", transformMatrix);

                modelComponent.updateImpl();
            }

            shadowMapCubeAtlas.unbind();
        }

        _distanceShaderProgram.stopUsing();
    }
}

void RenderingSystem::renderSpotLightShadowMaps()
{
    SHKYERA_PROFILE("RenderingSystem::renderSpotLightShadowMaps");

    glEnable(GL_DEPTH_TEST);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    std::unordered_set<Entity> spotLightEntities;
    for(const auto& [entity, spotLightComponent] : _registry->getComponentSet<SpotLightComponent>()) {
        spotLightEntities.insert(entity);
        if(_spotLightToShadowMap.find(entity) == _spotLightToShadowMap.end())
        {
            _spotLightToShadowMap.emplace(entity, DepthAtlasFrameBuffer(1));
            auto& newShadowMap = _spotLightToShadowMap.at(entity);
            
            newShadowMap.bind();
            newShadowMap.setSize(1500, 1500);
            newShadowMap.clear();
            newShadowMap.unbind();
        }
    }

    std::vector<Entity> entitiesWithFrameBuffersToRemove;
    for(const auto& [entity, _buffer] : _spotLightToShadowMap)
    {
        if(spotLightEntities.find(entity) == spotLightEntities.end())
        {
            entitiesWithFrameBuffersToRemove.push_back(entity);
        }
    }

    for(const auto& entityToRemove : entitiesWithFrameBuffersToRemove)
    {
        _spotLightToShadowMap.erase(entityToRemove);
    }

    for(auto& [lightEntity, shadowMap] : _spotLightToShadowMap)
    {
        shadowMap.bind();
        shadowMap.clear();
        shadowMap.unbind();

        const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(lightEntity, _registry);
        const auto lightPosition = glm::vec3{lightTransformMatrix[3]};
        const auto& lightRange = _registry->getComponent<SpotLightComponent>(lightEntity).range;
        const auto& lightOuterCutoff = _registry->getComponent<SpotLightComponent>(lightEntity).outerCutoff;
        const auto lightSpaceMatrix = _registry->getComponent<SpotLightComponent>(lightEntity).getLightSpaceMatrix(lightTransformMatrix);

        _distanceShaderProgram.use();

        shadowMap.bind(0);
        _distanceShaderProgram.setUniform("projectionViewMatrix", lightSpaceMatrix);
        for (const auto& [modelEntity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
            const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
            _distanceShaderProgram.setUniform("modelMatrix", transformMatrix);
            modelComponent.updateImpl();
        }
        shadowMap.unbind();

        _distanceShaderProgram.stopUsing();
    }
}

void RenderingSystem::renderWorldObjects(const PostProcessingVolumeComponent& postProcessing)
{
    SHKYERA_PROFILE("RenderingSystem::renderWorldObjects");

    glEnable(GL_DEPTH_TEST);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraTransformMatrix = TransformComponent::getGlobalTransformMatrix(_registry->getCamera(), _registry);
    const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    // ********* Rendering the world *********
    UseShader modelShaderUsage(_modelShaderProgram);
    _mostRecentFrameBufferPtr->bind();

    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _modelShaderProgram.setUniform("projectionViewMatrix", projectionMatrix * viewMatrix);
    _modelShaderProgram.setUniform("viewPos", cameraPosition);

    glm::vec3 ambientColor{0, 0, 0};
    for(const auto& ambientLightComponent : _registry->getComponents<AmbientLightComponent>())
    {
        ambientColor += ambientLightComponent.color * ambientLightComponent.intensity;
    }
    _modelShaderProgram.setUniform("ambientLight", ambientColor);

    int pointLightIndex = 0;
    for(const auto& [entity, pointLightComponent] : _registry->getComponentSet<PointLightComponent>()) {
        auto& depthCubeMap = _pointLightToShadowMap.at(entity);
        depthCubeMap.getTexture().activate(GL_TEXTURE0 + _textureIndex);
        _modelShaderProgram.setUniform("pointLightsDepthCubeMap[" + std::to_string(pointLightIndex) + "]", _textureIndex);
        ++_textureIndex;

        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].position", transformComponent.getPosition());
        _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].color", pointLightComponent.intensity * pointLightComponent.color);
        _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].range", pointLightComponent.range);
        ++pointLightIndex;
    }
    _modelShaderProgram.setUniform("numPointLights", pointLightIndex);

    int directionalLightIndex = 0;
    for (const auto& [entity, directionalLightComponent] : _registry->getComponentSet<DirectionalLightComponent>())
    {
        const auto& lightTransform = _registry->getComponent<TransformComponent>(entity);
        const auto& depthAtlasFrameBuffer = _directionalLightToShadowMap.at(entity);
        const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);

        for(size_t levelOfDetail = 0; levelOfDetail < DirectionalLightComponent::LevelsOfDetail; levelOfDetail++)
        {
            const glm::mat4 lightSpaceMatrix = directionalLightComponent.getLightSpaceMatrix(lightTransformMatrix, cameraTransform, cameraComponent, levelOfDetail);
            _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].lightSpaceMatrix[" + std::to_string(levelOfDetail) + "]", lightSpaceMatrix);
        }

        depthAtlasFrameBuffer.getTexture().activate(GL_TEXTURE0 + _textureIndex);
        _modelShaderProgram.setUniform("directionalLightsDepthMap[" + std::to_string(directionalLightIndex) + "]", _textureIndex);
        ++_textureIndex;

        glm::vec3 lightDirection = DirectionalLightComponent::getDirection(lightTransformMatrix);
        _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].direction", lightDirection);
        _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].color", directionalLightComponent.intensity * directionalLightComponent.color);
        ++directionalLightIndex;
    }
    _modelShaderProgram.setUniform("numDirectionalLights", directionalLightIndex);

    int spotLightIndex = 0;
    for(const auto& [entity, spotLightComponent] : _registry->getComponentSet<SpotLightComponent>())
    {
        {
            const auto& lightTransform = _registry->getComponent<TransformComponent>(entity);
            const auto lightTransformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
            const auto globalLightPosition = glm::vec3{lightTransformMatrix[3]};
            const auto lightDirection = glm::normalize(glm::vec3{1, 0, 0} * glm::mat3{glm::inverse(lightTransformMatrix)});
            const auto lightSpaceMatrix = _registry->getComponent<SpotLightComponent>(entity).getLightSpaceMatrix(lightTransformMatrix);

            _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].lightSpaceMatrix", lightSpaceMatrix);
            _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].position", globalLightPosition);
            _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].direction", lightDirection);
            _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].color", spotLightComponent.intensity * spotLightComponent.color);
            _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].range", spotLightComponent.range);
            _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].innerCutoffCosine", std::cos(spotLightComponent.innerCutoff));
            _modelShaderProgram.setUniform("spotLights[" + std::to_string(spotLightIndex) + "].outerCutoffCosine", std::cos(spotLightComponent.outerCutoff));
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

    { // SSAO
        _ssaoFrameBuffer.getTexture().activate(GL_TEXTURE0 + _textureIndex);
        _modelShaderProgram.setUniform("ssao", _textureIndex);
        ++_textureIndex;

        _modelShaderProgram.setUniform("viewportSize", _mostRecentFrameBufferPtr->getSize());
    }

    {
        SHKYERA_PROFILE("RenderingSystem::renderWorldObjects - Models");
        for (const auto& [entity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
            const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
            _modelShaderProgram.setUniform("modelMatrix", transformMatrix);

            const auto& material = std::get<AssetRef<Material>>(modelComponent.material);
            setMaterial(material.get());

            modelComponent.updateImpl();
        }
    }
}

void RenderingSystem::renderParticles()
{
    SHKYERA_PROFILE("RenderingSystem::renderParticles");
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    UseShader modelShaderUsage(_modelShaderProgram);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraTransformMatrix = TransformComponent::getGlobalTransformMatrix(_registry->getCamera(), _registry);
    const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);

    static const auto billboardPlane = utils::assets::fromFactory<Mesh, &Mesh::Factory::createPlane>(_registry.get());
    billboardPlane->bind();

    for (const auto& [entity, emitter] : _registry->getComponentSet<ParticleEmitterComponent>())
    {
        if(!emitter.enabled)
        {
            continue;
        }

        auto cameraFacingTransform = glm::mat4{1.0};
        cameraFacingTransform = utils::transform::getCameraFacingModelMatrix(cameraFacingTransform, cameraPosition, viewMatrix);

        const auto& material = std::get<AssetRef<Material>>(emitter.material);
        setMaterial(material.get());

        const auto& state = emitter.state;

        for(size_t i = 0; i < state.lifetimes.size(); ++i)
        {
            if(state.lifetimes[i] < 0)
            {
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

void RenderingSystem::renderBillboards()
{
    SHKYERA_PROFILE("RenderingSystem::renderBillboards");

    UseShader modelShaderUsage(_modelShaderProgram);
    _mostRecentFrameBufferPtr->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraTransformMatrix = TransformComponent::getGlobalTransformMatrix(_registry->getCamera(), _registry);
    const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);

    static const auto billboardPlane = utils::assets::fromFactory<Mesh, &Mesh::Factory::createPlane>(_registry.get());

    billboardPlane->bind();
    for (const auto& [entity, billboardComponent] : _registry->getComponentSet<BillboardComponent<RuntimeMode::PRODUCTION>>())
    {
        if(billboardComponent.occlusion == BillboardComponent<RuntimeMode::PRODUCTION>::Occlusion::Occludable)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        const auto& baseTransform = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        glm::mat4 modelMatrix = billboardComponent.getModelMatrix(baseTransform, cameraPosition, viewMatrix);
        _modelShaderProgram.setUniform("modelMatrix", modelMatrix);

        const auto& material = std::get<AssetRef<Material>>(billboardComponent.material);
        setMaterial(material.get());

        glDrawElements(GL_TRIANGLES, billboardPlane->getMeshSize(), GL_UNSIGNED_INT, nullptr);
    }

    const auto drawDebugBillboard = [this, &cameraPosition, &viewMatrix](const auto& componentSet, Material const* material) {
        static auto debugBillboard = BillboardComponent<>{
            .orientation = BillboardComponent<>::Orientation::CameraFacing,
            .scale = BillboardComponent<>::Scale::Camera,
            .occlusion = BillboardComponent<>::Occlusion::NotOccludable
        };
        
        for (const auto& [entity, _] : componentSet)
        {
            auto baseTransform = TransformComponent::getGlobalTransformMatrix(entity, _registry);

            glm::vec3 translation = glm::vec3(baseTransform[3]); // Extract translation
            glm::vec3 right   = glm::normalize(glm::vec3(baseTransform[0])); // X-axis
            glm::vec3 up      = glm::normalize(glm::vec3(baseTransform[1])); // Y-axis
            glm::vec3 forward = glm::normalize(glm::vec3(baseTransform[2])); // Z-axis

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
        drawDebugBillboard(_registry->getComponentSet<PointLightComponent>(), &_pointLightDebugMaterial); 
        drawDebugBillboard(_registry->getComponentSet<DirectionalLightComponent>(), &_directionalLightDebugMaterial); 
        drawDebugBillboard(_registry->getComponentSet<SpotLightComponent>(), &_spotLightDebugMaterial); 
        drawDebugBillboard(_registry->getComponentSet<AmbientLightComponent>(), &_ambientLightDebugMaterial); 
        drawDebugBillboard(_registry->getComponentSet<ParticleEmitterComponent>(), &_particleEmitterDebugMaterial); 
        drawDebugBillboard(_registry->getComponentSet<PostProcessingVolumeComponent>(), &_postProcessingVolumeDebugMaterial); 
    }
    billboardPlane->unbind();

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    _mostRecentFrameBufferPtr->unbind();
}


void RenderingSystem::renderPostProcessingVolumes()
{
    glEnable(GL_DEPTH_TEST);

    UseShader modelShaderUsage(_wireframeShaderProgram);
    _mostRecentFrameBufferPtr->bind();

    static const glm::vec3 volumeWireframeColor{0.8, 0.8, 0.8};
    _wireframeShaderProgram.setUniform("fixedColor", volumeWireframeColor);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();
    const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

    static const auto volumeCube = utils::assets::fromFactory<Wireframe, &Wireframe::Factory::createCube>(_registry.get());

    volumeCube->bind();
    for (const auto& [entity, _volumeComponent] : _registry->getComponentSet<PostProcessingVolumeComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        _wireframeShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
        glDrawArrays(GL_LINES, 0, volumeCube->getEdgeCount());
    }
    volumeCube->unbind();

    _mostRecentFrameBufferPtr->unbind();
}

void RenderingSystem::bloom(float threshold, float weight)
{
    SHKYERA_PROFILE("RenderingSystem::renderBloom");

    glDisable(GL_DEPTH_TEST);

    // Downscaling Pass
    utils::applyShaderToFrameBuffer(
        _downscaledFrameBuffers[0],
        _thresholdShaderProgram,
        {
            { "sceneTexture", &_mostRecentFrameBufferPtr->getTexture() }
        },
        utils::Uniform("threshold", threshold)
    );
    for(size_t i = 1; i < BloomSteps; ++i)
    {
        utils::applyShaderToFrameBuffer(
        _downscaledFrameBuffers[i],
        _thresholdShaderProgram,
        {
            { "sceneTexture", &_downscaledFrameBuffers[i - 1].getTexture() }
        },
        utils::Uniform("threshold", 0.0f)
    );
    }

    // Horizontal Blur Pass
    for(size_t i = 0; i < BloomSteps; ++i)
    {
        utils::applyShaderToFrameBuffer(
            _horizontallyBluredDownscaledFrameBuffers[i],
            _horizontalBlurShaderProgram,
            {
                { "textureToBlur", &_downscaledFrameBuffers[i].getTexture() }
            },
            utils::Uniform("resolution", _downscaledFrameBuffers[i].getSize())
        );
    }
    

    // Vertical Blur Pass
    for(size_t i = 0; i < BloomSteps; ++i)
    {
        utils::applyShaderToFrameBuffer(
            _fullyBluredDownscaledFrameBuffers[i],
            _verticalBlurShaderProgram,
            {
                { "textureToBlur", &_horizontallyBluredDownscaledFrameBuffers[i].getTexture() }
            },
            utils::Uniform("resolution", _horizontallyBluredDownscaledFrameBuffers[i].getSize())
        );
    }

    // Blur Combination Pass
    auto* currentBuffer = &_fullyBluredDownscaledFrameBuffers[BloomSteps - 1];
    float currentWeight = 1.0f / (1 << (BloomSteps - 1));

    for (int i = BloomSteps - 2; i >= 0; --i)
    {
        auto* nextBuffer = &_fullyBluredDownscaledFrameBuffers[i];
        float nextWeight = 1.0f / (1 << i);

        utils::applyShaderToFrameBuffer(
            *currentBuffer,
            _weightedAdditionShaderProgram,
            {
                { "firstTexture", &currentBuffer->getTexture() },
                { "secondTexture", &nextBuffer->getTexture() }
            },
            utils::Uniform("firstWeight", currentWeight),
            utils::Uniform("secondWeight", nextWeight)
        );

        currentWeight += nextWeight;
    }

    // Composite bloom with the original scene
    utils::applyShaderToFrameBuffer(
        _bloomedFrameBuffer,
        _weightedAdditionShaderProgram,
        {
            { "firstTexture", &_mostRecentFrameBufferPtr->getTexture() },
            { "secondTexture", &currentBuffer->getTexture() }
        },
        utils::Uniform("firstWeight", 1.0f),
        utils::Uniform("secondWeight", weight)
    );

    _mostRecentFrameBufferPtr = &_bloomedFrameBuffer;
}

void RenderingSystem::toneMapping()
{
    SHKYERA_PROFILE("RenderingSystem::toneMapping");

    glDisable(GL_DEPTH_TEST);

    utils::applyShaderToFrameBuffer(
        _toneMappedFrameBuffer,
        _toneMappingShaderProgram,
        {
            { "original", &_mostRecentFrameBufferPtr->getTexture() }
        }
    );

    _mostRecentFrameBufferPtr = &_toneMappedFrameBuffer;
}

void RenderingSystem::gammaCorrection(float gamma)
{
    SHKYERA_PROFILE("RenderingSystem::gammaCorrection");

    glDisable(GL_DEPTH_TEST);

    utils::applyShaderToFrameBuffer(
        _gammaCorrectedFrameBuffer,
        _gammaCorrectionShaderProgram,
        {
            { "original", &_mostRecentFrameBufferPtr->getTexture() }
        },
        utils::Uniform("gamma", gamma)
    );

    _mostRecentFrameBufferPtr = &_gammaCorrectedFrameBuffer;
}

void RenderingSystem::renderWireframes()
{
    SHKYERA_PROFILE("RenderingSystem::renderWireframes");

    glEnable(GL_DEPTH_TEST);

    _mostRecentFrameBufferPtr->bind();

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _wireframeShaderProgram.use();

    const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

    static const glm::vec3 wireframeColor{0.08, 0.7, 0.15};

    for (const auto& [entity, wireframeComponent] : _registry->getComponentSet<WireframeComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        _wireframeShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
        _wireframeShaderProgram.setUniform("fixedColor", wireframeColor);
        wireframeComponent.updateImpl();
    }

    _wireframeShaderProgram.stopUsing();
    _mostRecentFrameBufferPtr->unbind();
}

void RenderingSystem::renderSkybox()
{
    SHKYERA_PROFILE("RenderingSystem::renderSkybox");

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();
    static auto cubemap = utils::assets::fromFactory<Mesh, &Mesh::Factory::createCubeMap>(_registry.get());

    glDepthFunc(GL_LEQUAL);

    _mostRecentFrameBufferPtr->bind();
    _skyboxShaderProgram.use();
    _skyboxShaderProgram.setUniform("viewMatrix", glm::mat4(viewMatrix));
    _skyboxShaderProgram.setUniform("projectionMatrix", projectionMatrix);

    for(const auto& skybox : _registry->getComponents<SkyboxComponent>())
    {
        skybox.skyboxCubeMap.activate(GL_TEXTURE0);
        _skyboxShaderProgram.setUniform("skybox", 0);
        cubemap->draw();
    }
    
    _skyboxShaderProgram.stopUsing();
    _mostRecentFrameBufferPtr->unbind();

    glDepthFunc(GL_LESS);
}

void RenderingSystem::renderOverlayModels()
{
    SHKYERA_PROFILE("RenderingSystem::renderOverlayModels");

    glDisable(GL_DEPTH_TEST);

    _mostRecentFrameBufferPtr->bind();

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _wireframeShaderProgram.use();

    const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

    for (const auto& [entity, overlayModelComponent] : _registry->getComponentSet<OverlayModelComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        _wireframeShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
        _wireframeShaderProgram.setUniform("fixedColor", overlayModelComponent.getMaterial()->albedo);
        overlayModelComponent.updateImpl();
    }

    _wireframeShaderProgram.stopUsing();
    _mostRecentFrameBufferPtr->unbind();

    glEnable(GL_DEPTH_TEST);
}

void RenderingSystem::antiAliasing()
{
    SHKYERA_PROFILE("RenderingSystem::antiAliasing");
    utils::applyShaderToFrameBuffer(
        _antiAliasedFrameBuffer,
        _antiAliasingShaderProgram,
        {
            {"sceneTexture", &_mostRecentFrameBufferPtr->getTexture()}
        }
    );

    _mostRecentFrameBufferPtr = &_antiAliasedFrameBuffer;
}

void RenderingSystem::setTexture(Material const* material, const std::string& textureName, auto textureMember)
{
    UseShader modelShaderUsage(_modelShaderProgram);
    if(const auto& textureAsset = std::get<AssetRef<Texture>>((*material).*textureMember))
    {
        _modelShaderProgram.setUniform("material." + textureName + "TextureLoaded", true);

        textureAsset->activate(GL_TEXTURE0 + _textureIndex);
        _modelShaderProgram.setUniform("material." + textureName + "Texture", _textureIndex);
        ++_textureIndex;
    }
    else
    {
        _modelShaderProgram.setUniform("material." + textureName + "TextureLoaded", false);
    }
}


void RenderingSystem::setMaterial(Material const* material)
{
    UseShader modelShaderUsage(_modelShaderProgram);
    if(material == nullptr)
    {
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

PostProcessingVolumeComponent RenderingSystem::getPostProcessingSettings()
{
    static const auto volume = AABB{
        .center = {0, 0, 0},
        .extents = {1.0, 1.0, 1.0}
    };

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraTransformMatrix = TransformComponent::getGlobalTransformMatrix(_registry->getCamera(), _registry);
    const auto& cameraPosition = glm::vec3{cameraTransformMatrix[3]};

    for(const auto& [entity, postProcessingVolume] : _registry->getComponentSet<PostProcessingVolumeComponent>())
    {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        if(volume.isInside(transformMatrix, cameraPosition))
        {
            return postProcessingVolume;
        }
    }

    for(const auto& postProcessingVolume : _registry->getComponents<PostProcessingVolumeComponent>())
    {
        if(postProcessingVolume.global)
        {
            return postProcessingVolume;
        }
    }

    return PostProcessingVolumeComponent{};
}

}
