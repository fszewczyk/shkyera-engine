#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/DirectionalLightComponentUI.hpp>

namespace shkyera {

DirectionalLightComponentUI::DirectionalLightComponentUI(DirectionalLightComponent* directionalLightComponent) :
    _directionalLightComponent(directionalLightComponent) {}

void DirectionalLightComponentUI::draw() {
    ImGui::Image((ImTextureID)Image::ICON_COMPONENT_TRANSFORM.getTextureId(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
    glm::vec3& diffuse = _directionalLightComponent->diffuse;
    glm::vec3& specular = _directionalLightComponent->specular;

    ImGui::PushItemWidth(50);

    ImGui::Text("Diffuse");
    ImGui::SameLine(120);
    ImGui::DragFloat("R##Diffuse", &diffuse[0], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("G##Diffuse", &diffuse[1], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("B##Diffuse", &diffuse[2], 0.01f, 0.0f, 1.0f);

    ImGui::Text("Specular");
    ImGui::SameLine(120);
    ImGui::DragFloat("R##Specular", &specular[0], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("G##Specular", &specular[1], 0.01f, 0.0f, 1.0f);
    ImGui::SameLine();
    ImGui::DragFloat("B##Specular", &specular[2], 0.01f, 0.0f, 1.0f);

    ImGui::PopItemWidth();

    ImGui::TreePop();
  }
}

}
