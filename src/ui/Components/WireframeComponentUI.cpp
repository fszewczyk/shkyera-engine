#include "imgui.h"

#include <UI/Components/WireframeComponentUI.hpp>
#include <AssetManager/Image.hpp>

namespace shkyera {

WireframeComponentUI::WireframeComponentUI(std::shared_ptr<Registry> registry, WireframeComponent* wireframeComponent) :
  _registry(registry),
  _wireframeSelector("Mesh", registry, std::get<OptionalAssetHandle>(wireframeComponent->wireframe)), 
  _wireframeComponent(wireframeComponent) 
{
  _wireframeSelector.setUpdateCallback([this](const auto& assetHandle) {
    auto& meshAsset =_registry->getComponent<AssetComponent<Wireframe>>(assetHandle);
    _wireframeComponent->wireframe = HandleAndAsset<Wireframe>{assetHandle, utils::assets::read(meshAsset)};
  });
  
  _wireframeSelector.setClearCallback([this]() {
    _wireframeComponent->wireframe = HandleAndAsset<Wireframe>{std::nullopt, nullptr};
  });
}

void WireframeComponentUI::draw() {
  ImGui::Image(_icon->getImguiTextureID(), ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Wireframe", ImGuiTreeNodeFlags_DefaultOpen)) {
    _wireframeSelector.draw();
    ImGui::TreePop();
  }
}

}  // namespace shkyera
