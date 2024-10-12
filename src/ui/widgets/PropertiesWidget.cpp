#include "imgui.h"

#include <iostream>

#include <Components/NameComponent.hpp>
#include <UI/Widgets/PropertiesWidget.hpp>
#include <UI/Common/Style.hpp>

namespace shkyera {

void PropertiesWidget::selectEntity(Entity entity) {
    _selectedEntity = entity;
}

void PropertiesWidget::setRegistry(std::shared_ptr<Registry> registry) {
    _registry = registry;
}

void PropertiesWidget::draw() {
    ImGui::Begin(_name.c_str());

    if(_selectedEntity.has_value())
    {
        ImGui::PushID(*_selectedEntity);

        ImGui::PushFont(style::HUGE_FONT);
        ImGui::TextUnformatted(_registry->getComponent<NameComponent>(*_selectedEntity).getName().c_str());
        ImGui::PopFont();

        ImGui::Separator();

        drawNewComponentMenu();

        ImGui::PopID();
    }
    else 
    {
        ImGui::Text("No object has been selected.");
    }
    ImGui::End();
}

void PropertiesWidget::drawNewComponentMenu() {
    if (ImGui::Button("New Component", ImVec2(-1, 0)))
        ImGui::OpenPopup("Add Component");
    if (ImGui::BeginPopup("Add Component")) {
        if (ImGui::Selectable("Script")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}


} // namespace shkyera
