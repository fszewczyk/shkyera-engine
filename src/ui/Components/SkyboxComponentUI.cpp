#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/SkyboxComponentUI.hpp>

namespace shkyera {

SkyboxComponentUI::SkyboxComponentUI(SkyboxComponent* skyboxComponent) :
    _skyboxComponent(skyboxComponent) {}

void SkyboxComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Skybox", ImGuiTreeNodeFlags_DefaultOpen)) {
    glm::vec3& ambientLight = _skyboxComponent->ambientLight;

    ImGui::PushItemWidth(50);

    ImGui::Text("Skylight Color");
    ImGui::SameLine(120);
    ImGui::DragFloat("R##Skylight Color", &ambientLight[0], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("G##Skylight Color", &ambientLight[1], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("B##Skylight Color", &ambientLight[2], 0.01f, 0.0f, 1.0f);

    ImGui::PopItemWidth();

    ImGui::TreePop();
  }
}

}
