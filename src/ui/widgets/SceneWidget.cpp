#include <imgui_internal.h>
#include "imgui.h"

#include <glad/glad.h>

#include <iostream>
#include <AssetManager/Image.hpp>
#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Shader.hpp>
#include <Components/NameComponent.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/SceneWidget.hpp>


namespace shkyera {

SceneWidget::SceneWidget(std::shared_ptr<Registry> registry) : Widget("Scene"), _runtime(std::move(registry)) {}

void SceneWidget::draw() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(_name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    auto renderSize = ImGui::GetContentRegionAvail();
    _runtime.getRenderingSystem().setSize(renderSize.x, renderSize.y);
    _runtime.update();

    ImGui::Image((void*)(intptr_t)_runtime.getRenderingSystem().getTexture(), renderSize);

    ImGui::End();
    ImGui::PopStyleVar();
}

}  // namespace shkyera
