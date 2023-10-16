#include "imgui.h"
#include <imgui_internal.h>

#include <iostream>

#include "core/Filesystem.hpp"
#include "core/Image.hpp"
#include "python/Interpreter.hpp"
#include "ui/UI.hpp"
#include "ui/components/ScriptUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ScriptUIComponent::draw() {
    ImGui::PushID(_uuid);
    ImGui::Image((ImTextureID)Image::ICON_COMPONENT_SCRIPT.getTextureId(), ImVec2(16, 16));
    ImGui::SameLine();

    if (_script) {
        if (ImGui::TreeNodeEx((_script->getFile()->getName() + " (Script)").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            drawScriptFile();
            drawVariables();
            ImGui::TreePop();
        }
    } else {
        if (ImGui::TreeNodeEx("Empty Script", ImGuiTreeNodeFlags_DefaultOpen)) {
            drawScriptFile();
            ImGui::TreePop();
        }
    }
    ImGui::PopID();
}

void ScriptUIComponent::replaceScript(std::filesystem::path path) {
    if (!std::filesystem::exists(path)) {
        ConsoleWidget::logError("Path does not exist. Make sure that the file at " + path.string() +
                                " is still there.");
        return;
    }

    try {
        std::shared_ptr<File> scriptFile = Directory::getFile(path);

        if (_script) {
            ScriptComponent::removeScript(_script);
        }

        _script = ScriptComponent::addScript(_object, scriptFile);
    } catch (std::invalid_argument error) {
        ConsoleWidget::logError(error.what());
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

    if (_script == nullptr) {
        ImGui::BeginDisabled();
        ImGui::Text("Drag Script");
        ImGui::EndDisabled();
    } else {
        ImGui::Image((ImTextureID)Image::ICON_FILES_PYTHON.getTextureId(), ImVec2(16, 16));
        ImGui::SameLine();
        ImGui::Text(_script->getFile()->getName().c_str());
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DRAG_AND_DROP_SCRIPT")) {
            char payloadData[payload->DataSize + 1];
            memset(payloadData, 0, payload->DataSize + 1);
            memcpy(payloadData, (char *)payload->Data, payload->DataSize);

            std::string pathToScript(payloadData);
            std::filesystem::path path(pathToScript);

            replaceScript(path);
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

void ScriptUIComponent::drawVariables() {
    if (_script == nullptr)
        return;

    if (ImGui::Button("Reload", ImVec2(-1, 0))) {
        if (Python::isRunning())
            ConsoleWidget::logError("Cannot reload script while running.");
        else
            _script->update();
    }

    auto &floatVariables = _script->getFloatVariables();
    auto &intVariables = _script->getIntVariables();
    auto &stringVariables = _script->getStringVariables();
    auto &vec3Variables = _script->getVec3Variables();

    for (PublicFloat &variable : floatVariables)
        drawFloatVariable(variable);
    for (PublicInt &variable : intVariables)
        drawIntVariable(variable);
    for (PublicString &variable : stringVariables)
        drawStringVariable(variable);
    for (PublicVec3 &variable : vec3Variables)
        drawVec3Variable(variable);
}

void ScriptUIComponent::drawFloatVariable(PublicFloat &variable) {
    ImGui::Text(variable.name.c_str());

    ImGui::SameLine();
    ImGui::SetCursorPosX(LABEL_MARGIN);
    ImGui::DragFloat(("##" + variable.name).c_str(), &variable.value, 0.01f, 0.0f, 0.0f, "%.3f");
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
