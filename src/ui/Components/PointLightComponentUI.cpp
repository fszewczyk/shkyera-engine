#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/PointLightComponentUI.hpp>

namespace shkyera {

PointLightComponentUI::PointLightComponentUI(PointLightComponent* pointLightComponent) :
    _pointLightComponent(pointLightComponent) {}

void PointLightComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
    glm::vec3& diffuse = _pointLightComponent->diffuse;
    glm::vec3& specular = _pointLightComponent->specular;

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
