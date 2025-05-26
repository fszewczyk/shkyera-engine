#include "Components/Environment.hpp"
#include "Components/SelectedEntityComponent.hpp"
#include "imgui.h"

#include <iostream>

#include <Components/BillboardComponent.hpp>
#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraTags.hpp>
#include <Components/DirectionalLightComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/ParticleEmitterComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <Components/PostProcessingVolumeComponent.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/SpotLightComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>

#include <UI/Common/Style.hpp>
#include <UI/Components/AmbientLightComponentUI.hpp>
#include <UI/Components/BillboardComponentUI.hpp>
#include <UI/Components/CameraComponentUI.hpp>
#include <UI/Components/DirectionalLightComponentUI.hpp>
#include <UI/Components/ModelComponentUI.hpp>
#include <UI/Components/ParticleEmitterComponentUI.hpp>
#include <UI/Components/PointLightComponentUI.hpp>
#include <UI/Components/PostProcessingVolumeComponentUI.hpp>
#include <UI/Components/SpotLightComponentUI.hpp>
#include <UI/Components/TransformComponentUI.hpp>
#include <UI/Components/WireframeComponentUI.hpp>
#include <UI/Widgets/PropertiesWidget.hpp>

namespace shkyera {

PropertiesWidget::PropertiesWidget(std::shared_ptr<Registry> registry) : Widget("Properties"), _registry(registry) {}
PropertiesWidget::PropertiesWidget(std::shared_ptr<Registry> registry, const std::string& title)
    : Widget(title), _registry(registry) {}

void PropertiesWidget::updateComponents() {
  const auto& selectedSet = _registry->getComponentSet<SelectedEntityComponent>();
  if (selectedSet.size() > 0) {
    const auto [firstSelectedEntity, _] = *(selectedSet.begin());

    if (!_selectedEntity.has_value() || firstSelectedEntity != *_selectedEntity) {
      _selectedEntity = firstSelectedEntity;
      setupComponentsUI();
    }
  }
}

void PropertiesWidget::draw() {
  updateComponents();

  ImGui::Begin(_name.c_str());
  if (_selectedEntity.has_value()) {
    ImGui::PushID(*_selectedEntity);

    if (_registry->hasComponent<NameComponent>(*_selectedEntity)) {
      ImGui::PushFont(style::HUGE_FONT);
      ImGui::TextUnformatted(_registry->getComponent<NameComponent>(*_selectedEntity).getName().c_str());
      ImGui::PopFont();
      ImGui::Separator();
    }

    drawExistingComponents();
    drawNewComponentMenu();

    ImGui::PopID();
  } else {
    ImGui::Text("No object has been selected.");
  }

  ImGui::End();
}

void PropertiesWidget::drawExistingComponents() {
  for (const auto& component : _componentsUi) {
    component->draw();
  }
}

void PropertiesWidget::setupComponentsUI() {
  _componentsUi.clear();

  if (_registry->hasComponent<TransformComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<TransformComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<TransformComponentUI>();
    componentUi->setPositionGetter([&]() -> glm::vec3& { return component.getPosition(); });
    componentUi->setOrientationGetter([&]() -> glm::vec3& { return component.getOrientation(); });
    componentUi->setScaleGetter([&]() -> glm::vec3& { return component.getScale(); });
    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<CameraComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<CameraComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<CameraComponentUI>(&component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<ModelComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<ModelComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<ModelComponentUI>(_registry, &component);

    componentUi->setOnMeshUpdate([this]() {
      if (!_registry->hasComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(*_selectedEntity)) {
        _registry->addComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(*_selectedEntity);
      }
      const auto mesh = std::get<AssetRef<Mesh>>(_registry->getComponent<ModelComponent>(*_selectedEntity).mesh);
      auto& colliderComponent =
          _registry->getComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(*_selectedEntity);
      colliderComponent.box = mesh->getBoundingBox();
    });

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<BillboardComponent<RuntimeMode::PRODUCTION>>(*_selectedEntity)) {
    auto& component = _registry->getComponent<BillboardComponent<RuntimeMode::PRODUCTION>>(*_selectedEntity);
    auto componentUi = std::make_unique<BillboardComponentUI>(_registry, &component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<WireframeComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<WireframeComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<WireframeComponentUI>(_registry, &component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<AmbientLightComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<AmbientLightComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<AmbientLightComponentUI>(&component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<PointLightComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<PointLightComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<PointLightComponentUI>(&component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<SpotLightComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<SpotLightComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<SpotLightComponentUI>(&component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<DirectionalLightComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<DirectionalLightComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<DirectionalLightComponentUI>(&component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<PostProcessingVolumeComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<PostProcessingVolumeComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<PostProcessingVolumeComponentUI>(&component);

    _componentsUi.emplace_back(std::move(componentUi));
  }

  if (_registry->hasComponent<ParticleEmitterComponent>(*_selectedEntity)) {
    auto& component = _registry->getComponent<ParticleEmitterComponent>(*_selectedEntity);
    auto componentUi = std::make_unique<ParticleEmitterComponentUI>(_registry, &component);

    _componentsUi.emplace_back(std::move(componentUi));
  }
}

void PropertiesWidget::drawNewComponentMenu() {
  if (ImGui::Button("New Component", ImVec2(-1, 0))) {
    ImGui::OpenPopup("Add Component");
  }
  if (ImGui::BeginPopup("Add Component")) {
    auto addComponentIfSelected = [this]<typename ComponentType>(const std::string& label) {
      if (ImGui::Selectable(label.c_str())) {
        _registry->addComponent<ComponentType>(*_selectedEntity);
        setupComponentsUI();
        ImGui::CloseCurrentPopup();
      }
    };

    addComponentIfSelected.template operator()<TransformComponent>("Transform");
    addComponentIfSelected.template operator()<CameraComponent>("Camera");
    addComponentIfSelected.template operator()<ModelComponent>("Model");
    addComponentIfSelected.template operator()<BillboardComponent<RuntimeMode::PRODUCTION>>("Billboard");
    addComponentIfSelected.template operator()<WireframeComponent>("Wireframe");
    addComponentIfSelected.template operator()<AmbientLightComponent>("Ambient Light");
    addComponentIfSelected.template operator()<PointLightComponent>("Point Light");
    addComponentIfSelected.template operator()<SpotLightComponent>("Spot Light");
    addComponentIfSelected.template operator()<DirectionalLightComponent>("Directional Light");
    addComponentIfSelected.template operator()<PostProcessingVolumeComponent>("Post-Processing Volume");
    addComponentIfSelected.template operator()<ParticleEmitterComponent>("Particle Emitter");

    ImGui::EndPopup();
  }
}

CameraPropertiesWidget::CameraPropertiesWidget(std::shared_ptr<Registry> registry)
    : PropertiesWidget(registry, "Scene Camera") {}

void CameraPropertiesWidget::updateComponents() {
  static auto onlyOnce = [this]() {
    SHKYERA_ASSERT(_registry->getEntity<SceneCamera>().has_value(),
                   "SceneCamera is not registered. Cannot use CameraPropertiesWidget");
    _selectedEntity = *_registry->getEntity<SceneCamera>();
    setupComponentsUI();
    return true;
  }();
}

void CameraPropertiesWidget::drawNewComponentMenu() {}

EnvironmentPropertiesWidget::EnvironmentPropertiesWidget(std::shared_ptr<Registry> registry)
    : PropertiesWidget(registry, "Environment") {
  SHKYERA_ASSERT(registry->getEntity<Environment>().has_value(),
                 "Environment component is not registered. Cannot create an EnvironmentPropertiesWidget");
}

void EnvironmentPropertiesWidget::updateComponents() {
  static auto onlyOnce = [this]() {
    _selectedEntity = *_registry->getEntity<Environment>();
    setupComponentsUI();
    return true;
  }();
}

void EnvironmentPropertiesWidget::drawNewComponentMenu() {}

}  // namespace shkyera
