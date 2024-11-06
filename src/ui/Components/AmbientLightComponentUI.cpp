#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/AmbientLightComponentUI.hpp>

namespace shkyera {

AmbientLightComponentUI::AmbientLightComponentUI(AmbientLightComponent* ambientLightComponent) :
    _colorSelector("Color", ambientLightComponent->color), 
    _intensitySlider("Intensity", ambientLightComponent->intensity, 0.0f, 5.0f), 
    _ambientLightComponent(ambientLightComponent) 
{
    _colorSelector.setUpdateCallback([this](const glm::vec3& color) {
      _ambientLightComponent->color = color;
    });
    _intensitySlider.setUpdateCallback([this](float value) {
      _ambientLightComponent->intensity = value;
    });
}

void AmbientLightComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Ambient Light", ImGuiTreeNodeFlags_DefaultOpen)) {
    _colorSelector.draw();
    _intensitySlider.draw();

    ImGui::TreePop();
  }
}

}
