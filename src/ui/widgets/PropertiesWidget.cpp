#include "imgui.h"

#include "ui/UI.hpp"
#include "ui/widgets/PropertiesWidget.hpp"

#include "ui/components/ScriptUIComponent.hpp"

namespace shkyera {

void PropertiesWidget::draw() {
    ImGui::Begin(_name.c_str());

    if (_object) {
        ImGui::PushID(_object.get());

        ImGui::PushFont(UI::HUGE_FONT);
        ImGui::Text(_object->getName().c_str());
        ImGui::PopFont();

        ImGui::Separator();

        for (std::shared_ptr<UIComponent> comp : _components) {
            comp->draw();
        }

        ImGui::Separator();

        drawNewComponentMenu();

        ImGui::PopID();
    } else {
        ImGui::Text("No object has been selected.");
    }

    ImGui::End();
}

void PropertiesWidget::setObject(std::shared_ptr<GameObject> object) {
    _object = object;
    _components = UIComponent::getComponentsOfObject(_object);
}

void PropertiesWidget::drawNewComponentMenu() {
    if (ImGui::Button("New Component", ImVec2(-1, 0)))
        ImGui::OpenPopup("Add Component");
    if (ImGui::BeginPopup("Add Component")) {
        if (ImGui::Selectable("Script")) {
            UIComponent::addComponentToObject(_object, std::make_shared<ScriptUIComponent>("Script", _object));
            setObject(_object);

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

std::shared_ptr<GameObject> PropertiesWidget::_object = nullptr;
std::vector<std::shared_ptr<UIComponent>> PropertiesWidget::_components = {};

} // namespace shkyera