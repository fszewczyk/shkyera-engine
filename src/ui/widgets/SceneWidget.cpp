#include <imgui_internal.h>
#include "imgui.h"

#include <glad/glad.h>

#include <iostream>
#include <AssetManager/Image.hpp>
#include <InputManager/InputManager.hpp>
#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Shader.hpp>
#include <Components/NameComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/SceneWidget.hpp>


namespace shkyera {

SceneWidget::SceneWidget(std::shared_ptr<Registry> registry) : Widget("Scene"), _registry(registry), _runtime(std::move(registry)) {}

void SceneWidget::draw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(_name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    auto renderSize = ImGui::GetContentRegionAvail();
    updateWindowCoordinateSystem();
    _runtime.getRenderingSystem().setSize(renderSize.x * 2.0f, renderSize.y * 2.0f);
    const auto aspectRatio = static_cast<float>(renderSize.x) / renderSize.y;
    _registry->getComponent<CameraComponent>(_registry->getCamera()).aspectRatio = aspectRatio;

    _runtime.update();

    ImGui::Image((void*)(intptr_t)_runtime.getRenderingSystem().getRenderFrameBuffer(), renderSize);
    
    ImGui::End();
    ImGui::PopStyleVar();
}

void SceneWidget::updateWindowCoordinateSystem() {
    ImVec2 topLeft = ImGui::GetWindowContentRegionMin();
    ImVec2 topRight = ImGui::GetWindowContentRegionMax();

    topLeft.x += ImGui::GetWindowPos().x;
    topLeft.y += ImGui::GetWindowPos().y;
    topRight.x += ImGui::GetWindowPos().x;
    topRight.y += ImGui::GetWindowPos().y;

    const glm::vec2 topLeftVec = {topLeft.x, topLeft.y};
    const glm::vec2 bottomRightVec = {topRight.x, topRight.y};

    InputManager::getInstance().setCoordinateSystem(InputManager::CoordinateSystem::SCENE, topLeftVec, bottomRightVec);
}


}  // namespace shkyera
