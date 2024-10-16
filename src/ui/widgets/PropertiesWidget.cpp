#include "imgui.h"

#include <iostream>

#include <Components/NameComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/MeshComponent.hpp>
#include <Components/CameraComponent.hpp>

#include <UI/Common/Style.hpp>
#include <UI/Components/TransformComponentUI.hpp>
#include <UI/Components/MeshComponentUI.hpp>
#include <UI/Components/CameraComponentUI.hpp>
#include <UI/Widgets/PropertiesWidget.hpp>

namespace shkyera {

PropertiesWidget::PropertiesWidget(std::shared_ptr<Registry> registry) : Widget("Properties"), _registry(registry) {}

void PropertiesWidget::selectEntity(Entity entity) {
  _selectedEntity = entity;
  setupComponentsUI();
}

void PropertiesWidget::draw() {
  ImGui::Begin(_name.c_str());

  if (_selectedEntity.has_value()) {
    ImGui::PushID(*_selectedEntity);

    ImGui::PushFont(style::HUGE_FONT);
    ImGui::TextUnformatted(
        _registry->getComponent<NameComponent>(*_selectedEntity)
            .getName()
            .c_str());
    ImGui::PopFont();

    ImGui::Separator();

    drawExistingComponents();
    drawNewComponentMenu();

    ImGui::PopID();
  } else {
    ImGui::Text("No object has been selected.");
  }
  ImGui::End();
}

void PropertiesWidget::drawExistingComponents() {
  for (const auto& component : _componentsUi)
    component->draw();
}

void PropertiesWidget::setupComponentsUI() {
  _componentsUi.clear();

  if(_registry->hasComponent<TransformComponent>(*_selectedEntity)) {    
    auto &component = _registry->getComponent<TransformComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<TransformComponentUI>();
    componentUi->setPositionGetter([&]() -> glm::vec3& { return component.getPosition(); });
    componentUi->setOrientationGetter([&]() -> glm::vec3& { return component.getOrientation(); });
    componentUi->setScaleGetter([&]() -> glm::vec3& { return component.getScale(); });
    _componentsUi.emplace_back(std::move(componentUi));
  }

  if(_registry->hasComponent<MeshComponent>(*_selectedEntity)) {    
    auto &component = _registry->getComponent<MeshComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<MeshComponentUI>();
    
    _componentsUi.emplace_back(std::move(componentUi));
  }

  if(_registry->hasComponent<CameraComponent>(*_selectedEntity)) {    
    auto &component = _registry->getComponent<CameraComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<CameraComponentUI>();
    
    _componentsUi.emplace_back(std::move(componentUi));
  }
}

void PropertiesWidget::drawNewComponentMenu() {
  if (ImGui::Button("New Component", ImVec2(-1, 0))) {
    ImGui::OpenPopup("Add Component");
  }
  if (ImGui::BeginPopup("Add Component")) {
    if (ImGui::Selectable("Transform")) {
      _registry->addComponent<TransformComponent>(*_selectedEntity);
      setupComponentsUI();
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::Selectable("Texture")) {
      _registry->addComponent<MeshComponent>(*_selectedEntity);
      setupComponentsUI();
      ImGui::CloseCurrentPopup();
    }

    if (ImGui::Selectable("Camera")) {
      _registry->addComponent<CameraComponent>(*_selectedEntity);
      setupComponentsUI();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

}  // namespace shkyera
