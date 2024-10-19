#include "imgui.h"

#include <UI/Components/ModelComponentUI.hpp>

#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Filesystem.hpp>
#include <AssetManager/Image.hpp>

namespace shkyera {

ModelComponentUI::ModelComponentUI(ModelComponent* modelComponent) :
  _meshSelector("Mesh"), _materialSelector("Material"), _modelComponent(modelComponent) {
  const auto& defaultMeshFilePath = AssetManager::getInstance().getFilePath(modelComponent->getMesh());
  if(defaultMeshFilePath) {
    _meshSelector.setFile(File(std::filesystem::path(*defaultMeshFilePath)));
  }
  _meshSelector.setUpdateCallback([this](const auto& file) {
    _modelComponent->setMesh(AssetManager::getInstance().getAsset<Mesh>(file.getPath()));
  });
  _meshSelector.setClearCallback([this]() {
    _modelComponent->setMesh(nullptr);
  });

  const auto& defaultMaterialFilePath = AssetManager::getInstance().getFilePath(modelComponent->getMaterial());
  if(defaultMaterialFilePath) {
    _materialSelector.setFile(File(std::filesystem::path(*defaultMaterialFilePath)));
  }
  _materialSelector.setClearCallback([this]() {
    _modelComponent->setMaterial(nullptr);
  });
}

void ModelComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_FILES_IMAGE.getTextureId(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
    _meshSelector.draw();
    _materialSelector.draw();
    ImGui::TreePop();
  }
}

}  // namespace shkyera
