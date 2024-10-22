#include "imgui.h"

#include <UI/Components/WireframeComponentUI.hpp>
#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Filesystem.hpp>
#include <AssetManager/Image.hpp>

namespace shkyera {

WireframeComponentUI::WireframeComponentUI(WireframeComponent* wireframeComponent) :
  _wireframeSelector("Mesh"), _wireframeComponent(wireframeComponent) {
  
  const auto& defaultWireframeFilePath = AssetManager::getInstance().getFilePath(wireframeComponent->getWireframe());
  if (defaultWireframeFilePath) {
    _wireframeSelector.setFile(File(std::filesystem::path(*defaultWireframeFilePath)));
  }

  _wireframeSelector.setUpdateCallback([this](const auto& file) {
    _wireframeComponent->setWireframe(AssetManager::getInstance().getAsset<Wireframe>(file.getPath()));
  });
  
  _wireframeSelector.setClearCallback([this]() {
    _wireframeComponent->setWireframe(nullptr);
  });
}

void WireframeComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_FILES_IMAGE.getTextureId(), ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Wireframe", ImGuiTreeNodeFlags_DefaultOpen)) {
    _wireframeSelector.draw();
    ImGui::TreePop();
  }
}

}  // namespace shkyera
