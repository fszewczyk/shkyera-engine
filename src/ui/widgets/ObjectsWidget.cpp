#include "imgui.h"
#include <imgui_internal.h>

#include <UI/widgets/ConsoleWidget.hpp>
#include <UI/widgets/ObjectsWidget.hpp>
#include <UI/widgets/PropertiesWidget.hpp>
#include <Components/NameComponent.hpp>

namespace shkyera {

ObjectsWidget::ObjectsWidget(std::string name) : Widget(name) {
    addOnSelectEntityCallback([this](Entity e) { _selectedEntity = e; });
}

void ObjectsWidget::setRegistry(std::shared_ptr<Registry> registry) { _registry = registry; }

void ObjectsWidget::addOnNewEntityCallback(std::function<void(Entity)> callback) { _onNewEntityCallbacks.emplace_back(callback); }

void ObjectsWidget::addOnSelectEntityCallback(std::function<void(Entity)> callback) { _onSelectEntityCallbacks.emplace_back(callback); }

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
            const auto newEntity = _registry->addEntity();
            _registry->addComponent<NameComponent>(newEntity);

            for(const auto& callback : _onNewEntityCallbacks)
            {
                callback(newEntity);        
            }

            ConsoleWidget::logSuccess("Created Empty Object");
        }

        ImGui::EndPopup();
    }
}

void ObjectsWidget::drawList() {
    size_t i = 0;
    for(const auto& [entity, nameComponent] : _registry->getComponentSet<NameComponent>())
    {
        if (ImGui::Selectable((nameComponent.getName() + "##" + std::to_string(i++)).c_str(), entity == _selectedEntity)) {
            for(const auto& callback : _onSelectEntityCallbacks)
            {
                callback(entity);        
            }
        }
    }
}

} // namespace shkyera
