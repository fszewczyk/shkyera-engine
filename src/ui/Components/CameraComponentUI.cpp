#include <UI/Components/CameraComponentUI.hpp>

namespace shkyera {

void CameraComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_COMPONENT_TRANSFORM.getTextureId(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::TreePop();
  }
}

}  // namespace shkyera
