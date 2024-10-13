#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/TriangleComponentUI.hpp>

namespace shkyera {

void TriangleComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_FILES_IMAGE.getTextureId(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Triangle", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Triangle");
    ImGui::TreePop();
  }
}

}  // namespace shkyera
