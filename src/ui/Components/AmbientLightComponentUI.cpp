#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/AmbientLightComponentUI.hpp>

namespace shkyera {

AmbientLightComponentUI::AmbientLightComponentUI(AmbientLightComponent* ambientLightComponent) :
    _ambientLightComponent(ambientLightComponent) {}

void AmbientLightComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Ambient Light", ImGuiTreeNodeFlags_DefaultOpen)) {
    glm::vec3& ambientLight = _ambientLightComponent->ambient;

    ImGui::PushItemWidth(50);

    ImGui::Text("Ambient Light Color");
    ImGui::SameLine(120);
    ImGui::DragFloat("R##Ambient Light Color", &ambientLight[0], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("G##Ambient Light Color", &ambientLight[1], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("B##Ambient Light Color", &ambientLight[2], 0.01f, 0.0f, 1.0f);

    ImGui::PopItemWidth();

    ImGui::TreePop();
  }
}

}
