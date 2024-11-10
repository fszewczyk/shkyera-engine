#include <Systems/RenderingSystem.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Rendering/Utils.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/AmbientLightComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <Components/DirectionalLightComponent.hpp>

namespace shkyera {

RenderingSystem::RenderingSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    const auto& positionAndNormalVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/position_and_normal.glsl", Shader::Type::Vertex);
    const auto& modelFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/uber.glsl", Shader::Type::Fragment);
    _modelShaderProgram.attachShader(positionAndNormalVertexShader);
    _modelShaderProgram.attachShader(modelFragmentShader);
    _modelShaderProgram.link();

    const auto& fixedColorFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/color.glsl", Shader::Type::Fragment);
    _wireframeShaderProgram.attachShader(positionAndNormalVertexShader);
    _wireframeShaderProgram.attachShader(fixedColorFragmentShader);
    _wireframeShaderProgram.link();

    const auto& positionVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/position.glsl", Shader::Type::Vertex);
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
}

void RenderingSystem::renderOutline(const std::unordered_set<Entity>& entities)
{
    glDisable(GL_DEPTH_TEST);

    // Drawing a silhouette
    _silhouetteFrameBuffer.bind();
    _silhouetteFrameBuffer.clear();
    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();
    _silhouetteShaderProgram.use();
    _silhouetteShaderProgram.setUniform("viewMatrix", viewMatrix);
    _silhouetteShaderProgram.setUniform("projectionMatrix", projectionMatrix);
    _silhouetteShaderProgram.setUniform("fixedColor", glm::vec3{1.0, 0.1, 1.0});
    for (const auto& entity : entities) {
        if(_registry->hasComponents<TransformComponent, ModelComponent>(entity))
        {
            const auto& modelComponent = _registry->getComponent<ModelComponent>(entity);
            const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
            _silhouetteShaderProgram.setUniform("modelMatrix", transformComponent.getTransformMatrix());
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

void RenderingSystem::renderModels()
{
    glEnable(GL_DEPTH_TEST);

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const auto& cameraComponent = _registry->getComponent<CameraComponent>(_registry->getCamera());

    // ********* Rendering the shadow maps *********
    std::unordered_set<Entity> directionalLightEntities;
    for(const auto& [entity, directionalLightComponent] : _registry->getComponentSet<DirectionalLightComponent>()) {
        directionalLightEntities.insert(entity);
        if(_directionalLightToShadowMaps.find(entity) == _directionalLightToShadowMaps.end())
        {
            _directionalLightToShadowMaps.emplace(entity, std::vector<DepthFrameBuffer>(DirectionalLightComponent::LevelsOfDetail));
        }
    }

    std::vector<Entity> entitiesWithFrameBuffersToRemove;
    for(const auto& [entity, _buffer] : _directionalLightToShadowMaps)
    {
        if(directionalLightEntities.find(entity) == directionalLightEntities.end())
        {
            entitiesWithFrameBuffersToRemove.push_back(entity);
        }
    }

    for(const auto& entityToRemove : entitiesWithFrameBuffersToRemove)
    {
        _directionalLightToShadowMaps.erase(entityToRemove);
    }

    for(auto& [lightEntity, lodBuffers] : _directionalLightToShadowMaps)
    {
        uint8_t levelOfDetail = 0;
        lodBuffers.resize(DirectionalLightComponent::LevelsOfDetail);
        for(auto& buffer : lodBuffers)
        {
            buffer.bind();
            buffer.setSize(1600, 1600);
            buffer.clear();
            _shadowMapShaderProgram.use();

            const auto& directionalLightComponent = _registry->getComponent<DirectionalLightComponent>(lightEntity);
            const auto& lightTransform = _registry->getComponent<TransformComponent>(lightEntity);
            const glm::mat4& lightSpaceMatrix = directionalLightComponent.getLightSpaceMatrix(lightTransform, cameraTransform, cameraComponent, levelOfDetail);

            _shadowMapShaderProgram.setUniform("lightSpaceMatrix", lightSpaceMatrix);
            for (const auto& [modelEntity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
                const auto& transformComponent = _registry->getComponent<TransformComponent>(modelEntity);
                _shadowMapShaderProgram.setUniform("modelMatrix", transformComponent.getTransformMatrix());

                modelComponent.updateImpl();
            }

            _shadowMapShaderProgram.stopUsing();
            buffer.unbind();
            levelOfDetail++;
        }
    }        

    // ********* Rendering the world *********
    _renderFrameBuffer.bind();

    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _modelShaderProgram.use();
    _modelShaderProgram.setUniform("viewMatrix", viewMatrix);
    _modelShaderProgram.setUniform("projectionMatrix", projectionMatrix);
    _modelShaderProgram.setUniform("viewPos", cameraTransform.getPosition());

    glm::vec3 ambientColor{0, 0, 0};
    for(const auto& ambientLightComponent : _registry->getComponents<AmbientLightComponent>())
    {
        ambientColor += ambientLightComponent.color * ambientLightComponent.intensity;
    }
    _modelShaderProgram.setUniform("ambientLight", ambientColor);

    int pointLightIndex = 0;
    for(const auto& [entity, pointLightComponent] : _registry->getComponentSet<PointLightComponent>()) {
        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].position", transformComponent.getPosition());
        _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].color", pointLightComponent.intensity * pointLightComponent.color);
        _modelShaderProgram.setUniform("pointLights[" + std::to_string(pointLightIndex) + "].range", pointLightComponent.range);
        ++pointLightIndex;
    }
    _modelShaderProgram.setUniform("numPointLights", pointLightIndex);

    int directionalLightIndex = 0;
    int textureIndex = 0;
    for (const auto& [entity, directionalLightComponent] : _registry->getComponentSet<DirectionalLightComponent>()) {
        const auto& lightTransform = _registry->getComponent<TransformComponent>(entity);
        const auto& orientation = lightTransform.getOrientation();

        const auto& depthFrameBuffers = _directionalLightToShadowMaps.at(entity);

        for(size_t levelOfDetail = 0; levelOfDetail < depthFrameBuffers.size(); levelOfDetail++)
        {
            depthFrameBuffers[levelOfDetail].getTexture().activate(GL_TEXTURE0 + textureIndex);
            const glm::mat4 lightSpaceMatrix = directionalLightComponent.getLightSpaceMatrix(lightTransform, cameraTransform, cameraComponent, levelOfDetail);

            _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].shadowSampler[" + std::to_string(levelOfDetail) + "]", textureIndex);
            _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].lightSpaceMatrix[" + std::to_string(levelOfDetail) + "]", lightSpaceMatrix);
            ++textureIndex;
        }


        glm::vec3 lightDirection = DirectionalLightComponent::getDirection(lightTransform);
        _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].direction", lightDirection);
        _modelShaderProgram.setUniform("directionalLights[" + std::to_string(directionalLightIndex) + "].color", directionalLightComponent.intensity * directionalLightComponent.color);
        ++directionalLightIndex;
    }
    _modelShaderProgram.setUniform("numDirectionalLights", directionalLightIndex);

    for (const auto& [entity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _modelShaderProgram.setUniform("modelMatrix", transformComponent.getTransformMatrix());

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
    _wireframeShaderProgram.setUniform("viewMatrix", viewMatrix);
    _wireframeShaderProgram.setUniform("projectionMatrix", projectionMatrix);

    static const glm::vec3 wireframeColor{0.08, 0.7, 0.15};

    for (const auto& [entity, wireframeComponent] : _registry->getComponentSet<WireframeComponent>()) {
        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _wireframeShaderProgram.setUniform("modelMatrix", transformComponent.getTransformMatrix());
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

}
