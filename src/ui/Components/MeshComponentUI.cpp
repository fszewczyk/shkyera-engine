#include <UI/Components/MeshComponentUI.hpp>

namespace shkyera {

void MeshComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_FILES_IMAGE.getTextureId(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Model");
    ImGui::TreePop();
  }
}

void MeshComponentUI::setPathGetter(std::function<std::string&(void)> getter) {
  _pathGetter = getter;
}

}  // namespace shkyera
