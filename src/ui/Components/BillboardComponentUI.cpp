#include "imgui.h"

#include <UI/Components/BillboardComponentUI.hpp>
#include <Components/BoxColliderComponent.hpp>

namespace shkyera {

BillboardComponentUI::BillboardComponentUI(std::shared_ptr<Registry> registry, BillboardComponent<>* billboardComponent) :
  _billboardComponent(billboardComponent), 
  _registry(registry), 
  _materialSelector("Material", registry.get(), std::get<OptionalAssetHandle>(_billboardComponent->material)),
  _orientationSelector("Orientation", _billboardComponent->orientation, {
    { BillboardComponent<>::Orientation::CameraFacing, "Camera Facing" },
    { BillboardComponent<>::Orientation::AxisAligned, "Axis Aligned" },
    { BillboardComponent<>::Orientation::WorldFixed, "World Fixed" }
  }),
  _scaleSelector("Scale", _billboardComponent->scale, {
    { BillboardComponent<>::Scale::Camera, "Camera Fixed" },
    { BillboardComponent<>::Scale::World, "World Fixed" }
  }),
  _occlusionSelector("Occlusion", _billboardComponent->occlusion, {
    { BillboardComponent<>::Occlusion::Occludable, "Occludable" },
    { BillboardComponent<>::Occlusion::NotOccludable, "Not Occludable" }
  })
{  
  _materialSelector.setUpdateCallback([this](const auto& assetHandle) {
    if(_registry->hasComponent<AssetComponent<Material>>(assetHandle))
    {
      auto& materialAsset =_registry->getComponent<AssetComponent<Material>>(assetHandle);
      _billboardComponent->material = HandleAndAsset<Material>{assetHandle, utils::assets::read(materialAsset)};
    }
  });
  _materialSelector.setClearCallback([this]() {
    _billboardComponent->material = HandleAndAsset<Material>{std::nullopt, nullptr};
  });

  auto setUpdateCallback = [this](auto& selector, auto billboardProperty) mutable {
      selector.setUpdateCallback([this, billboardProperty](const auto& val) {
          (_billboardComponent->*billboardProperty) = val;
      });
  };

  setUpdateCallback(_orientationSelector, &BillboardComponent<>::orientation);
  setUpdateCallback(_scaleSelector, &BillboardComponent<>::scale);
  setUpdateCallback(_occlusionSelector, &BillboardComponent<>::occlusion);
}

void BillboardComponentUI::draw() {
  ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Billboard", ImGuiTreeNodeFlags_DefaultOpen)) {
    _materialSelector.draw();
    _orientationSelector.draw();
    _scaleSelector.draw();
    _occlusionSelector.draw();
    ImGui::TreePop();
  }
}

}  // namespace shkyera
