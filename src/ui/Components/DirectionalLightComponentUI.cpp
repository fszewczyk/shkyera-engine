#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/DirectionalLightComponentUI.hpp>

namespace shkyera {

DirectionalLightComponentUI::DirectionalLightComponentUI(DirectionalLightComponent* directionalLightComponent)
    : ComponentUI("resources/icons/components/directional_light.png"),
      _directionalLightComponent(directionalLightComponent),
      _colorSelector("Color", directionalLightComponent->color),
      _intensitySlider("Intensity", directionalLightComponent->intensity, 0.0f, 5.0f) {
  _colorSelector.setUpdateCallback([this](const glm::vec3& color) { _directionalLightComponent->color = color; });
  _intensitySlider.setUpdateCallback([this](float value) { _directionalLightComponent->intensity = value; });
}

void DirectionalLightComponentUI::draw() {
  ImGui::Image(_icon->getImguiTextureID(), ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Directional Light", ImGuiTreeNodeFlags_DefaultOpen)) {
    _colorSelector.draw();
    _intensitySlider.draw();

    ImGui::TreePop();
  }
}

}  // namespace shkyera
