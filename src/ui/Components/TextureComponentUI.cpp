#include <UI/Components/TextureComponentUI.hpp>

namespace shkyera {

void TextureComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_FILES_IMAGE.getTextureId(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Text("Image");
    ImGui::TextUnformatted(_pathGetter().c_str());
    ImGui::TreePop();
  }
}

void TextureComponentUI::setPathGetter(std::function<std::string&(void)> getter) {
  _pathGetter = getter;
}

}  // namespace shkyera
