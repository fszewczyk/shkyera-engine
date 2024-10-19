#include "imgui.h"

#include <UI/Common/FileSelector.hpp>
#include <UI/Common/Style.hpp>

namespace shkyera {

FileSelector::FileSelector(const std::string& title) : _title(title) {}

void FileSelector::setFile(const File& file) {
    _file = file;
}

const std::optional<File>& FileSelector::getFile() const {
    return _file;
}

void FileSelector::setUpdateCallback(std::function<void(const File& file)> callback) {
    _updateCallback = callback;
}

void FileSelector::setClearCallback(std::function<void()> callback) {
    _clearCallback = callback;
}

void FileSelector::draw() {
    ImGui::TextUnformatted(_title.c_str());

    ImGui::SameLine();
    ImGui::SetCursorPosX(5);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7.0f, 5.0f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, style::DARK_ACCENT);
    ImGui::BeginChild("Mesh Child", ImVec2(-60, 24), true,
                      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    if (_file.has_value()) {
       ImGui::TextUnformatted(_file->getName().c_str());
    } else {
        ImGui::BeginDisabled();

        std::string label = "Drag " + _title;
        ImGui::TextUnformatted(label.c_str());
        ImGui::EndDisabled();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DRAG_AND_DROP_FILE")) {
            char payloadData[payload->DataSize + 1];
            memset(payloadData, 0, payload->DataSize + 1);
            memcpy(payloadData, (char *)payload->Data, payload->DataSize);

            std::string path(payloadData);
            _file = File(path);
            if(_updateCallback) {
                _updateCallback(*_file);
            }
        }
        ImGui::EndDragDropTarget();
    }


    ImGui::EndChild();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::SameLine();
    if(ImGui::Button("Clear", ImVec2(50, 22))) {
        _file = std::nullopt;
        if(_clearCallback) {
            _clearCallback();
        }
    }
}

}
