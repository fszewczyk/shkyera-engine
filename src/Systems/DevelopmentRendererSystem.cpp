#include <Systems/DevelopmentRendererSystem.hpp>
#include <AssetManager/AssetManager.hpp>

#include <Components/ModelComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/TransformComponent.hpp>

namespace shkyera {

DevelopmentRendererSystem::DevelopmentRendererSystem(std::shared_ptr<Registry> registry) : _registry(registry)
{

}


void DevelopmentRendererSystem::render()
{
    for(const auto& entity : _registry->getSelectedEntities())
    {
        if(_registry->hasComponent<ModelComponent>(entity))
        {
            drawOutline(entity);
        }
    }
}

void DevelopmentRendererSystem::drawOutline(Entity entity)
{
    const auto& cameraTransform = _registry->getComponent<TransformComponent>(_registry->getCamera());
    const glm::mat4& viewMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getViewMatrix(cameraTransform);
    const glm::mat4& projectionMatrix = _registry->getComponent<CameraComponent>(_registry->getCamera()).getProjectionMatrix();

    auto& entityModel = _registry->getComponent<ModelComponent>(entity);
    const auto& entityTransform = _registry->getComponent<TransformComponent>(entity);

    const float cameraToEntityDistance = glm::length(cameraTransform.getPosition() - entityTransform.getPosition());

    _outlineShaderProgram.use();
    _outlineShaderProgram.setUniform("viewMatrix", viewMatrix);
    _outlineShaderProgram.setUniform("projectionMatrix", projectionMatrix);
    _outlineShaderProgram.setUniform("modelMatrix", entityTransform.getTransformMatrix());
    _outlineShaderProgram.setUniform("outlineColor", glm::vec3(1.0f, 0.5f, 0.0f));
    _outlineShaderProgram.setUniform("outlineWidth", 0.1f);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  // Set stencil to 1 for the outline's location
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);  // Clear stencil buffer

    // --- Pass: Draw outline only where stencil is NOT filled ---
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);   // Draw only where stencil is NOT 1
    glStencilMask(0x00);                   // Disable writing to stencil buffer
    glDisable(GL_DEPTH_TEST);              // Disable depth test to overlay outline

    // Render the outline by scaling vertices along normals (vertex shader handles scaling)
    entityModel.update();

    // Reset states
    _outlineShaderProgram.stopUsing();
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xFF); 
    glDisable(GL_STENCIL_TEST);
}

}
