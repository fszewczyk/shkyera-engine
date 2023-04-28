#include "imgui.h"
#include <imgui_internal.h>

#include "ui/widgets/ConsoleWidget.hpp"
#include "ui/widgets/ObjectsWidget.hpp"

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
            _game->addGameObject(std::make_shared<GameObject>("Empty"));
            _gameObjects = _game->getGameObjects();
            ConsoleWidget::logInfo("Empty");
        }
        if (ImGui::Selectable("Fluid")) {
            _game->addGameObject(std::make_shared<GameObject>("Fluid"));
            _gameObjects = _game->getGameObjects();
            ConsoleWidget::logInfo("Fluid");
        }

        ImGui::EndPopup();
    }
}

void ObjectsWidget::drawList() {
    for (size_t i = 0; i < _gameObjects.size(); ++i) {
        if (ImGui::Selectable((_gameObjects[i]->getName() + "##" + std::to_string(i)).c_str(),
                              _selectedGameObject == i))
            _selectedGameObject = i;
    }
}

} // namespace shkyera
