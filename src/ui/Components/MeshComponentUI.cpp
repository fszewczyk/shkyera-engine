#include <UI/Components/MeshComponentUI.hpp>
#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Filesystem.hpp>
#include <AssetManager/Image.hpp>

namespace shkyera {

MeshComponentUI::MeshComponentUI(MeshComponent* meshComponent) :
  _fileSelector("Model"), _meshComponent(meshComponent) {

  const auto& defaultFilePath = AssetManager::getInstance().getFilePath(meshComponent->getMesh());
  if(defaultFilePath) {
    _fileSelector.setFile(File(std::filesystem::path(*defaultFilePath)));
  }
  _fileSelector.setUpdateCallback([this](const auto& file) {
    _meshComponent->setMesh(AssetManager::getInstance().getAsset<Mesh>(file.getPath()));
  });
  _fileSelector.setClearCallback([this]() {
    _meshComponent->setMesh(nullptr);
  });
}

void MeshComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_FILES_IMAGE.getTextureId(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
    _fileSelector.draw();
    ImGui::TreePop();
  }
}

}  // namespace shkyera
