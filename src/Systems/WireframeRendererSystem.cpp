#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Shader.hpp>
#include <Systems/WireframeRendererSystem.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/CameraComponent.hpp>

namespace shkyera {

WireframeRendererSystem::WireframeRendererSystem(std::shared_ptr<Registry> registry)
    : _registry(registry) {
    
    auto wireframeVertexShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/vertex/model.glsl", Shader::Type::Vertex);
    auto wireframeFragmentShader = AssetManager::getInstance().getAsset<Shader>("resources/shaders/fragment/color.glsl", Shader::Type::Fragment);

    _wireframeShaderProgram.attachShader(wireframeVertexShader);
    _wireframeShaderProgram.attachShader(wireframeFragmentShader);
    _wireframeShaderProgram.link();
}

void WireframeRendererSystem::render() {
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
}

}
