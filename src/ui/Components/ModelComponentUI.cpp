#include "imgui.h"

#include <UI/Components/ModelComponentUI.hpp>
#include <Components/BoxColliderComponent.hpp>
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
    const auto& mesh = AssetManager::getInstance().getAsset<Mesh>(file.getPath());
    _modelComponent->setMesh(mesh);
    if(_onMeshUpdate) {
      _onMeshUpdate(mesh);
    }
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

void ModelComponentUI::setOnMeshUpdate(std::function<void(std::shared_ptr<Mesh>)> callback) {
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
