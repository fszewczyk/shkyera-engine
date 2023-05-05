#include "imgui.h"
#include <imgui_internal.h>

#include "core/Image.hpp"
#include "ui/UI.hpp"
#include "ui/components/ScriptUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ScriptUIComponent::draw() {
    ImGui::Image((ImTextureID)Image::ICON_COMPONENT_SCRIPT.getTextureId(), ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNode(_name.c_str())) {
        drawScriptFile();
        ImGui::TreePop();
    }
}

void ScriptUIComponent::drawScriptFile() {
    ImGui::Text("Script");

    ImGui::SameLine();
    ImGui::SetCursorPosX(130);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7.0f, 5.0f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, UI::DARK_ACCENT);
    ImGui::BeginChild("Script Child", ImVec2(-1, 26), true,
                      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    if (_path.empty()) {
        ImGui::BeginDisabled();
        ImGui::Text("Drag Script");
        ImGui::EndDisabled();
    } else {
        ImGui::Image((ImTextureID)Image::ICON_FILES_PYTHON.getTextureId(), ImVec2(16, 16));
        ImGui::SameLine();
        ImGui::Text(_path.filename().string().c_str());
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DRAG_AND_DROP_SCRIPT")) {
            std::string pathToScript((char *)payload->Data);

            _path = std::filesystem::path(pathToScript);

            ConsoleWidget::logVerbose("Dropped " + pathToScript);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

} // namespace shkyera
