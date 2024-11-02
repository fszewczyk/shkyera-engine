#include <Systems/RenderingSystem.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Rendering/Utils.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/PointLightComponent.hpp>

namespace shkyera {

RenderingSystem::RenderingSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    const auto& positionAndNormalVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/position_and_normal.glsl", Shader::Type::Vertex);
    const auto& positionVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/position.glsl", Shader::Type::Vertex);
    const auto& texCoordsVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/texcoords.glsl", Shader::Type::Vertex);
    const auto& modelFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/uber.glsl", Shader::Type::Fragment);
    const auto& fixedColorFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/color.glsl", Shader::Type::Fragment);
    const auto& dilateFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/dilate.glsl", Shader::Type::Fragment);
    const auto& subtractFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/subtract.glsl", Shader::Type::Fragment);
    const auto& overlayFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/overlay.glsl", Shader::Type::Fragment);

    _modelShaderProgram.attachShader(positionAndNormalVertexShader);
    _modelShaderProgram.attachShader(modelFragmentShader);
    _modelShaderProgram.link();

    _wireframeShaderProgram.attachShader(positionAndNormalVertexShader);
    _wireframeShaderProgram.attachShader(fixedColorFragmentShader);
    _wireframeShaderProgram.link();

    _silhouetteShaderProgram.attachShader(positionVertexShader);
    _silhouetteShaderProgram.attachShader(fixedColorFragmentShader);
    _silhouetteShaderProgram.link();

    _dilateShaderProgram.attachShader(texCoordsVertexShader);
    _dilateShaderProgram.attachShader(dilateFragmentShader);
    _dilateShaderProgram.link();

    _subtractShaderProgram.attachShader(texCoordsVertexShader);
    _subtractShaderProgram.attachShader(subtractFragmentShader);
    _subtractShaderProgram.link();

    _overlayShaderProgram.attachShader(texCoordsVertexShader);
    _overlayShaderProgram.attachShader(overlayFragmentShader);
    _overlayShaderProgram.link();
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
    renderModels();
    renderWireframes();
    renderOutline(_registry->getSelectedEntities());
}

void RenderingSystem::renderOutline(const std::vector<Entity>& entities)
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
        const auto& modelComponent = _registry->getComponent<ModelComponent>(entity);
        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _silhouetteShaderProgram.setUniform("modelMatrix", transformComponent.getTransformMatrix());
        modelComponent.updateImpl();
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

    _renderFrameBuffer.bind();

    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    _modelShaderProgram.use();
    _modelShaderProgram.setUniform("viewMatrix", viewMatrix);
    _modelShaderProgram.setUniform("projectionMatrix", projectionMatrix);
    _modelShaderProgram.setUniform("viewPos", cameraTransform.getPosition());

    int lightIndex = 0;
    for(const auto& [entity, pointLightComponent] : _registry->getComponentSet<PointLightComponent>()) {
        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _modelShaderProgram.setUniform("lights[" + std::to_string(lightIndex) + "].position", transformComponent.getPosition());
        _modelShaderProgram.setUniform("lights[" + std::to_string(lightIndex) + "].ambient", pointLightComponent.ambient);
        _modelShaderProgram.setUniform("lights[" + std::to_string(lightIndex) + "].diffuse", pointLightComponent.diffuse);
        _modelShaderProgram.setUniform("lights[" + std::to_string(lightIndex) + "].specular", pointLightComponent.specular);
        ++lightIndex;
    }
    _modelShaderProgram.setUniform("numLights", lightIndex);

    for (const auto& [entity, modelComponent] : _registry->getComponentSet<ModelComponent>()) {
        const auto& transformComponent = _registry->getComponent<TransformComponent>(entity);
        _modelShaderProgram.setUniform("modelMatrix", transformComponent.getTransformMatrix());

        const Material* material = modelComponent.getMaterial();
        if (material) {
            _modelShaderProgram.setUniform("material.diffuse", material->getDiffuseColor());
            _modelShaderProgram.setUniform("material.specular", material->getSpecularColor());
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

}
