#include "imgui.h"
#include <imgui_internal.h>

#include <iostream>

#include "core/Image.hpp"
#include "game/components/ScriptComponent.hpp"
#include "python/Interpreter.hpp"
#include "ui/widgets/ConsoleWidget.hpp"
#include "ui/widgets/SceneWidget.hpp"

namespace shkyera {

void SceneWidget::setRenderer(std::shared_ptr<Renderer> renderer) {
    _renderer = renderer;
    Python::setInterpreterRenderer(_renderer);
}

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

void SceneWidget::adjustSize() {
    _renderSize = ImGui::GetWindowSize();
    _renderSize[0] -= 16;
    _renderSize[1] -= 64;

    _renderer->setDimension(_renderSize[0], _renderSize[1]);
}

void SceneWidget::drawRuntime() const {
    _renderer->draw();

    ImGui::BeginDisabled();
    ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_PLAY.getTextureId(), ImVec2(16, 16));
    ImGui::EndDisabled();

    ImGui::SameLine();

    if (ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_STOP.getTextureId(), ImVec2(16, 16))) {
        Python::stop();
    }

    ImGui::Image((ImTextureID)_renderer->getTextureId(), _renderSize);
}

void SceneWidget::drawScene() {
    if (ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_PLAY.getTextureId(), ImVec2(16, 16))) {
        adjustSize();
        ScriptComponent::moveScripts();
        Python::start();
    }
    ImGui::SameLine();

    ImGui::BeginDisabled();
    ImGui::ImageButton((ImTextureID)Image::ICON_BUTTON_STOP.getTextureId(), ImVec2(16, 16));
    ImGui::EndDisabled();
}

} // namespace shkyera
