#include "imgui.h"
#include <imgui_internal.h>

#include <iostream>

#include "core/Image.hpp"
#include "game/components/ScriptComponent.hpp"
#include "python/Interpreter.hpp"
#include "ui/widgets/ConsoleWidget.hpp"
#include "ui/widgets/SceneWidget.hpp"

namespace shkyera {

void SceneWidget::draw() {
    ImGui::Begin(_name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::Dummy(ImVec2(ImGui::GetWindowSize()[0] / 2 - 48, 16));
    ImGui::SameLine();

    if (Python::isRunning())
        drawRuntime();
    else
        drawScene();

    ImGui::End();
}

void SceneWidget::drawRuntime() const {
    ImGui::BeginDisabled();
    ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_PLAY.getTextureId(), ImVec2(16, 16));
    ImGui::EndDisabled();

    ImGui::SameLine();

    if (ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_STOP.getTextureId(), ImVec2(16, 16))) {
        Python::stop();
    }

    ImVec2 renderSize = ImGui::GetWindowSize();
    renderSize[0] -= 16;
    renderSize[1] -= 64;
    ImGui::Image((ImTextureID)Image::ICON_FILES_FOLDER.getTextureId(), renderSize);
}

void SceneWidget::drawScene() const {
    if (ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_PLAY.getTextureId(), ImVec2(16, 16))) {
        ScriptComponent::moveScripts();
        Python::start();
    }
    ImGui::SameLine();

    ImGui::BeginDisabled();
    ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_STOP.getTextureId(), ImVec2(16, 16));
    ImGui::EndDisabled();
}

} // namespace shkyera
