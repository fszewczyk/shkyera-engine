#include "imgui.h"

#include <UI/Components/ModelComponentUI.hpp>
#include <Components/BoxColliderComponent.hpp>

namespace shkyera {

ModelComponentUI::ModelComponentUI(std::shared_ptr<Registry> registry, ModelComponent* modelComponent) :
  _modelComponent(modelComponent), 
  _registry(registry), 
  _meshSelector("Mesh", registry.get(), std::get<OptionalAssetHandle>(modelComponent->mesh)), 
  _materialSelector("Material", registry.get(), std::get<OptionalAssetHandle>(modelComponent->material))
{  
  _meshSelector.setUpdateCallback([this](const auto& assetHandle) {
    if(_registry->hasComponent<AssetComponent<Mesh>>(assetHandle))
    {
      auto& meshAsset =_registry->getComponent<AssetComponent<Mesh>>(assetHandle);
      _modelComponent->mesh = HandleAndAsset<Mesh>{assetHandle, utils::assets::read(meshAsset)};
      if(_onMeshUpdate) {
        _onMeshUpdate();
      }
    }
  });
  
  _meshSelector.setClearCallback([this]() {
    _modelComponent->mesh = HandleAndAsset<Mesh>{std::nullopt, nullptr};
  });
  
  _materialSelector.setUpdateCallback([this](const auto& assetHandle) {
    if(_registry->hasComponent<AssetComponent<Material>>(assetHandle))
    {
      auto& materialAsset =_registry->getComponent<AssetComponent<Material>>(assetHandle);
      _modelComponent->material = HandleAndAsset<Material>{assetHandle, utils::assets::read(materialAsset)};
    }
  });
  
  _materialSelector.setClearCallback([this]() {
    _modelComponent->material = HandleAndAsset<Material>{std::nullopt, nullptr};
  });
}

void ModelComponentUI::setOnMeshUpdate(std::function<void()> callback) {
  _onMeshUpdate = callback;
}

void ModelComponentUI::draw() {
  ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
    _meshSelector.draw();
    _materialSelector.draw();
    ImGui::TreePop();
  }
}

}  // namespace shkyera
