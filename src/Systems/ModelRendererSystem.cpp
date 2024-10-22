#include <Systems/ModelRendererSystem.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/PointLightComponent.hpp>

namespace shkyera {

ModelRendererSystem::ModelRendererSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    auto modelVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/model.glsl", Shader::Type::Vertex);
    auto modelFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/uber.glsl", Shader::Type::Fragment);

    _modelShaderProgram.attachShader(modelVertexShader);
    _modelShaderProgram.attachShader(modelFragmentShader);
    _modelShaderProgram.link();
}

void ModelRendererSystem::render() {
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
}

}
