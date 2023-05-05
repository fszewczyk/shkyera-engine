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
        drawVariables();
        ImGui::TreePop();
    }
}

void ScriptUIComponent::drawScriptFile() {
    ImGui::Text("Script");

    ImGui::SameLine();
    ImGui::SetCursorPosX(LABEL_MARGIN);

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

            _floatVariables.push_back({"Floating", 4.0f});
            _intVariables.push_back({"Integer", 4});
            _stringVariables.push_back({"Text", ""});
            _vec3Variables.push_back({"Vector 3", {0, 0, 0}});

            _path = std::filesystem::path(pathToScript);

            ConsoleWidget::logVerbose("Dropped " + pathToScript);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void ScriptUIComponent::drawVariables() {
    for (PublicFloat &variable : _floatVariables)
        drawFloatVariable(variable);
    for (PublicInt &variable : _intVariables)
        drawIntVariable(variable);
    for (PublicString &variable : _stringVariables)
        drawStringVariable(variable);
    for (PublicVec3 &variable : _vec3Variables)
        drawVec3Variable(variable);
}

void ScriptUIComponent::drawFloatVariable(PublicFloat &variable) {
    ImGui::Text(variable.name.c_str());

    ImGui::SameLine();
    ImGui::SetCursorPosX(LABEL_MARGIN);
    ImGui::DragFloat(("##" + variable.name).c_str(), &variable.value, 0.01f);
}

void ScriptUIComponent::drawIntVariable(PublicInt &variable) {
    ImGui::Text(variable.name.c_str());

    ImGui::SameLine();
    ImGui::SetCursorPosX(LABEL_MARGIN);
    ImGui::DragInt(("##" + variable.name).c_str(), &variable.value);
}

void ScriptUIComponent::drawStringVariable(PublicString &variable) {
    ImGui::Text(variable.name.c_str());

    static char buffer[128] = "";

    ImGui::SameLine();
    ImGui::SetCursorPosX(LABEL_MARGIN);
    ImGui::InputText(("##" + variable.name).c_str(), buffer, 128);

    variable.value = std::string(buffer);
}

void ScriptUIComponent::drawVec3Variable(PublicVec3 &variable) {
    ImGui::Text(variable.name.c_str());

    ImGui::SameLine();
    ImGui::SetCursorPosX(LABEL_MARGIN);
    ImGui::PushItemWidth(50);

    ImGui::DragFloat(("X##" + variable.name).c_str(), &variable.value[0], 0.01f);
    ImGui::SameLine();
    ImGui::DragFloat(("Y##" + variable.name).c_str(), &variable.value[1], 0.01f);
    ImGui::SameLine();
    ImGui::DragFloat(("Z##" + variable.name).c_str(), &variable.value[2], 0.01f);

    ImGui::PopItemWidth();
}

} // namespace shkyera
