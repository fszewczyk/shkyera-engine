#include "imgui.h"

#include "ui/UI.hpp"
#include "ui/widgets/PropertiesWidget.hpp"

namespace shkyera {

void PropertiesWidget::draw() {
    ImGui::Begin(_name.c_str());

    if (_object) {
        ImGui::PushID(_object.get());

        ImGui::PushFont(UI::BIG_FONT);
        ImGui::Text(_object->getName().c_str());
        ImGui::PopFont();

        ImGui::Separator();

        for (std::shared_ptr<UIComponent> comp : _components) {
            comp->draw();
        }

        ImGui::PopID();
    } else {
        ImGui::Text("No object has been selected.");
    }

    ImGui::End();
}

void PropertiesWidget::setObject(std::shared_ptr<GameObject> object) {
    if (_object == object)
        return;

    _object = object;
    _components = UIComponent::getComponentsOfObject(_object);
}

std::shared_ptr<GameObject> PropertiesWidget::_object = nullptr;
std::vector<std::shared_ptr<UIComponent>> PropertiesWidget::_components = {};

} // namespace shkyera