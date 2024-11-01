#include <Systems/RenderingSystem.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/PointLightComponent.hpp>

namespace shkyera {

RenderingSystem::RenderingSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    const auto& vertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/model.glsl", Shader::Type::Vertex);
    const auto& modelFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/uber.glsl", Shader::Type::Fragment);
    const auto& wireframeFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/color.glsl", Shader::Type::Fragment);
    const auto& outlineVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/outline.glsl", Shader::Type::Vertex);
    const auto& outlineFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/outline.glsl", Shader::Type::Fragment);
    const auto& blurFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/blur.glsl", Shader::Type::Fragment);

    _modelShaderProgram.attachShader(vertexShader);
    _modelShaderProgram.attachShader(modelFragmentShader);
    _modelShaderProgram.link();

    _wireframeShaderProgram.attachShader(vertexShader);
    _wireframeShaderProgram.attachShader(wireframeFragmentShader);
    _wireframeShaderProgram.link();

    _outlineShaderProgram.attachShader(outlineVertexShader);
    _outlineShaderProgram.attachShader(outlineFragmentShader);
    _outlineShaderProgram.link();

    _blurShaderProgram.attachShader(outlineVertexShader);  // Reuse vertex shader
    _blurShaderProgram.attachShader(blurFragmentShader);
    _blurShaderProgram.link();
}


void RenderingSystem::setSize(uint32_t width, uint32_t height)
{
    _renderFrameBuffer.setSize(width, height);
    _silhouetteFrameBuffer.setSize(width, height);
    _blurredSilhouetteFrameBuffer.setSize(width / 2, height / 2);
}

GLuint RenderingSystem::getRenderFrameBuffer()
{
    return _renderFrameBuffer.getTexture().getID();
}

void RenderingSystem::render() {
    _renderFrameBuffer.bind();
    _renderFrameBuffer.clear();

    renderModels();
    renderWireframes();
}

void RenderingSystem::renderModels()
{
    _renderFrameBuffer.bind();
    glEnable(GL_DEPTH_TEST);

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
    _renderFrameBuffer.bind();
    glEnable(GL_DEPTH_TEST);

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
