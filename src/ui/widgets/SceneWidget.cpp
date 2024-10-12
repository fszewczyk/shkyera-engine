#include "imgui.h"
#include <imgui_internal.h>

#include <iostream>

#include <AssetManager/Image.hpp>
#include <UI/widgets/ConsoleWidget.hpp>
#include <UI/widgets/SceneWidget.hpp>

namespace shkyera {

void SceneWidget::draw() {
    ImGui::Begin(_name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::Dummy(ImVec2(ImGui::GetWindowSize()[0] / 2 - 48, 16));
    ImGui::SameLine();

    drawScene();

    ImGui::End();
}

void SceneWidget::adjustSize() {
    _renderSize = ImGui::GetWindowSize();
    _renderSize[0] -= 16;
    _renderSize[1] -= 64;
}

void SceneWidget::drawRuntime() const {
    readInput();

    ImGui::BeginDisabled();
    ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_PLAY.getTextureId(), ImVec2(16, 16));
    ImGui::EndDisabled();

    ImGui::SameLine();

    if (ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_STOP.getTextureId(), ImVec2(16, 16))) {
    }
}

void SceneWidget::drawScene() {
    if (ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_PLAY.getTextureId(), ImVec2(16, 16))) {
        adjustSize();
    }
    ImGui::SameLine();

    ImGui::BeginDisabled();
    ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_STOP.getTextureId(), ImVec2(16, 16));
    ImGui::EndDisabled();
}

void SceneWidget::readInput() const {

    for (int key = ImGuiKey_LeftArrow; key < ImGuiKey_KeypadEqual; ++key) {
        if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(key))) {
            const char *keyName = ImGui::GetKeyName(static_cast<ImGuiKey>(key));
        }
    }
}

} // namespace shkyera
