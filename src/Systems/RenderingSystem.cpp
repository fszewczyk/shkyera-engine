#include <Systems/RenderingSystem.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Rendering/Utils.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/OverlayModelComponent.hpp>
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
    const auto& positionAndNormalVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/position_and_normal.glsl", Shader::Type::Vertex);
    const auto& modelFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/uber.glsl", Shader::Type::Fragment);
    _modelShaderProgram.attachShader(positionAndNormalVertexShader);
    _modelShaderProgram.attachShader(modelFragmentShader);
    _modelShaderProgram.link();

    const auto& positionVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/position.glsl", Shader::Type::Vertex);
    const auto& fixedColorFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/color.glsl", Shader::Type::Fragment);
    _wireframeShaderProgram.attachShader(positionVertexShader);
    _wireframeShaderProgram.attachShader(fixedColorFragmentShader);
    _wireframeShaderProgram.link();

    _silhouetteShaderProgram.attachShader(positionVertexShader);
    _silhouetteShaderProgram.attachShader(fixedColorFragmentShader);
    _silhouetteShaderProgram.link();

    const auto& texCoordsVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/texcoords.glsl", Shader::Type::Vertex);
    const auto& dilateFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/dilate.glsl", Shader::Type::Fragment);
    _dilateShaderProgram.attachShader(texCoordsVertexShader);
    _dilateShaderProgram.attachShader(dilateFragmentShader);
    _dilateShaderProgram.link();

    const auto& subtractFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/subtract.glsl", Shader::Type::Fragment);
    _subtractShaderProgram.attachShader(texCoordsVertexShader);
    _subtractShaderProgram.attachShader(subtractFragmentShader);
    _subtractShaderProgram.link();

    const auto& overlayFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/overlay.glsl", Shader::Type::Fragment);
    _overlayShaderProgram.attachShader(texCoordsVertexShader);
    _overlayShaderProgram.attachShader(overlayFragmentShader);
    _overlayShaderProgram.link();

    const auto& skyboxVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/skybox.glsl", Shader::Type::Vertex);
    const auto& cubeMapFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/cubemap.glsl", Shader::Type::Fragment);
    _skyboxShaderProgram.attachShader(skyboxVertexShader);
    _skyboxShaderProgram.attachShader(cubeMapFragmentShader);
    _skyboxShaderProgram.link();

    const auto& shadowMapVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/shadowmap.glsl", Shader::Type::Vertex);
    const auto& depthFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/depth.glsl", Shader::Type::Fragment);
    _shadowMapShaderProgram.attachShader(shadowMapVertexShader);
    _shadowMapShaderProgram.attachShader(depthFragmentShader);
    _shadowMapShaderProgram.link();

    const auto& distanceVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/distance.glsl", Shader::Type::Vertex);
    const auto& distanceFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/distance.glsl", Shader::Type::Fragment);
    _shadowMapDistanceShaderProgram.attachShader(distanceVertexShader);
    _shadowMapDistanceShaderProgram.attachShader(distanceFragmentShader);
    _shadowMapDistanceShaderProgram.link();
}

void RenderingSystem::setSize(uint32_t width, uint32_t height)
{
    _renderFrameBuffer.setSize(width, height);
    _silhouetteFrameBuffer.setSize(width, height);
    _horizontallyDilatedFrameBuffer.setSize(width, height);
    _fullyDilatedFrameBuffer.setSize(width, height);
    _differenceFrameBuffer.setSize(width, height);
}

GLuint RenderingSystem::getRenderFrameBuffer()
{
    return _renderFrameBuffer.getTexture().getID();
    return _pointLightToShadowMap.begin()->second.getTexture().getID();
}

void RenderingSystem::render() {
    _renderFrameBuffer.bind();
    _renderFrameBuffer.clear();
    _renderFrameBuffer.unbind();

    // Main Rendering Pass
    renderSkybox();
    renderModels();
    renderWireframes();
    renderOutline(_registry->getSelectedEntities());
    renderGizmo();
}

void RenderingSystem::renderOutline(const std::unordered_set<Entity>& entities)
{
    if(entities.empty())
    {
        return;
    }

    for(const auto& entity : entities)
    {
        const auto& children = _registry->getHierarchy().getChildren(entity);
        renderOutline(children);
    }

    glDisable(GL_DEPTH_TEST);

    // Drawing a silhouette
    _silhouetteFrameBuffer.bind();
    _silhouetteFrameBuffer.clear();
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
        _renderFrameBuffer,
        _overlayShaderProgram,
        {
            { "background", &_renderFrameBuffer.getTexture() },
            { "overlay", &_differenceFrameBuffer.getTexture() }
        }
    );
}

void RenderingSystem::renderDirectionalLightShadowMaps()
{
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
            _shadowMapShaderProgram.use();

            const glm::mat4& lightSpaceMatrix = directionalLightComponent.getLightSpaceMatrix(lightTransformMatrix, cameraTransform, cameraComponent, levelOfDetail);

            _shadowMapShaderProgram.setUniform("lightSpaceMatrix", lightSpaceMatrix);
            for (const auto& [modelEntity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
                const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
                _shadowMapShaderProgram.setUniform("modelMatrix", transformMatrix);

                modelComponent.updateImpl();
            }

            _shadowMapShaderProgram.stopUsing();
            shadowMapAtlas.unbind();
        }
    }
}

void RenderingSystem::renderPointLightShadowMaps()
{
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

        _shadowMapDistanceShaderProgram.use();

        for(uint8_t face = 0; face < 6; ++face)
        {
            shadowMapCubeAtlas.bind(face);

            const glm::mat4& lightSpaceMatrix = captureViews[face];
            _shadowMapDistanceShaderProgram.setUniform("lightSpaceMatrix", lightSpaceMatrix);

            for (const auto& [modelEntity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
                const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
                _shadowMapDistanceShaderProgram.setUniform("modelMatrix", transformMatrix);

                modelComponent.updateImpl();
            }

            shadowMapCubeAtlas.unbind();
        }

        _shadowMapDistanceShaderProgram.stopUsing();
    }
}

void RenderingSystem::renderSpotLightShadowMaps()
{
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

        _shadowMapDistanceShaderProgram.use();

        shadowMap.bind(0);
        _shadowMapDistanceShaderProgram.setUniform("lightSpaceMatrix", lightSpaceMatrix);
        for (const auto& [modelEntity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
            const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(modelEntity, _registry);
            _shadowMapDistanceShaderProgram.setUniform("modelMatrix", transformMatrix);
            modelComponent.updateImpl();
        }
        shadowMap.unbind();

        _shadowMapDistanceShaderProgram.stopUsing();
    }
}

void RenderingSystem::renderModels()
{
    glEnable(GL_DEPTH_TEST);

    // ********* Rendering the shadow maps *********
    renderDirectionalLightShadowMaps();
    renderPointLightShadowMaps();
    renderSpotLightShadowMaps();

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    // ********* Rendering the world *********
    _renderFrameBuffer.bind();

    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _modelShaderProgram.use();
    _modelShaderProgram.setUniform("projectionViewMatrix", projectionMatrix * viewMatrix);
    _modelShaderProgram.setUniform("viewPos", cameraTransform.getPosition());
    constexpr float OneOverGamma = 1.0 / 2.2;
    _modelShaderProgram.setUniform("oneOverGamma", OneOverGamma);

    glm::vec3 ambientColor{0, 0, 0};
    for(const auto& ambientLightComponent : _registry->getComponents<AmbientLightComponent>())
    {
        ambientColor += ambientLightComponent.color * ambientLightComponent.intensity;
    }
    _modelShaderProgram.setUniform("ambientLight", ambientColor);

    int textureIndex = 0;
    int pointLightIndex = 0;
    for(const auto& [entity, pointLightComponent] : _registry->getComponentSet<PointLightComponent>()) {
        auto& depthCubeMap = _pointLightToShadowMap.at(entity);
        depthCubeMap.getTexture().activate(GL_TEXTURE0 + textureIndex);
        _modelShaderProgram.setUniform("pointLightsDepthCubeMap[" + std::to_string(pointLightIndex) + "]", textureIndex);
        ++textureIndex;

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

        depthAtlasFrameBuffer.getTexture().activate(GL_TEXTURE0 + textureIndex);
        _modelShaderProgram.setUniform("directionalLightsDepthMap[" + std::to_string(directionalLightIndex) + "]", textureIndex);
        ++textureIndex;

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
            depthBuffer.getTexture().activate(GL_TEXTURE0 + textureIndex);
            _modelShaderProgram.setUniform("spotLightsShadowMap[" + std::to_string(spotLightIndex) + "]", textureIndex);
            ++textureIndex;
        }

        ++spotLightIndex;
    }
    _modelShaderProgram.setUniform("numSpotLights", spotLightIndex);

    for (const auto& [entity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        _modelShaderProgram.setUniform("modelMatrix", transformMatrix);

        const Material* material = modelComponent.getMaterial();
        if (material) {
            _modelShaderProgram.setUniform("material.color", material->getDiffuseColor());
            _modelShaderProgram.setUniform("material.shininess", material->getShininess());
        }

        modelComponent.updateImpl();
    }

    _modelShaderProgram.stopUsing();
    _renderFrameBuffer.unbind();
}

void RenderingSystem::renderWireframes()
{
    glEnable(GL_DEPTH_TEST);

    _renderFrameBuffer.bind();

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
    _renderFrameBuffer.unbind();
}

void RenderingSystem::renderSkybox()
{
    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();
    static auto* cubemap = Mesh::Factory::createCubeMap();

    glDepthFunc(GL_LEQUAL);

    _renderFrameBuffer.bind();
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
    _renderFrameBuffer.unbind();

    glDepthFunc(GL_LESS);
}

void RenderingSystem::renderGizmo()
{
    glDisable(GL_DEPTH_TEST);

    _renderFrameBuffer.bind();

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _wireframeShaderProgram.use();

    const auto& projectionViewMatrix = projectionMatrix * viewMatrix;

    for (const auto& [entity, overlayModelComponent] : _registry->getComponentSet<OverlayModelComponent>()) {
        const auto& transformMatrix = TransformComponent::getGlobalTransformMatrix(entity, _registry);
        _wireframeShaderProgram.setUniform("projectionViewModelMatrix", projectionViewMatrix * transformMatrix);
        _wireframeShaderProgram.setUniform("fixedColor", overlayModelComponent.getMaterial()->getDiffuseColor());
        overlayModelComponent.updateImpl();
    }

    _wireframeShaderProgram.stopUsing();
    _renderFrameBuffer.unbind();

    glEnable(GL_DEPTH_TEST);
}

}
