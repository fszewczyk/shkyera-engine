#include "imgui.h"
#include <imgui_internal.h>

#include <UI/widgets/ConsoleWidget.hpp>
#include <UI/widgets/ObjectsWidget.hpp>
#include <UI/widgets/PropertiesWidget.hpp>

namespace shkyera {

void ObjectsWidget::setGame(std::shared_ptr<Game> game) { _game = game; }

void ObjectsWidget::draw() {
    ImGui::Begin(_name.c_str());
    drawCreate();
    ImGui::Separator();
    drawList();
    ImGui::End();
}

void ObjectsWidget::drawCreate() {
    if (ImGui::Button("Create"))
        ImGui::OpenPopup("Create Popup");
    if (ImGui::BeginPopup("Create Popup")) {
        if (ImGui::Selectable("Empty Object")) {
            ConsoleWidget::logSuccess("Created Empty Object");
        }

        ImGui::EndPopup();
    }
}

void ObjectsWidget::drawList() {
   
}

} // namespace shkyera
