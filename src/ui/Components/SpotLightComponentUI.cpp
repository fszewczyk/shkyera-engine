#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/SpotLightComponentUI.hpp>

namespace shkyera {

SpotLightComponentUI::SpotLightComponentUI(SpotLightComponent* pointLightComponent) :
    ComponentUI("resources/icons/components/spot_light.png"),
    _spotLightComponent(pointLightComponent),
    _colorSelector("Color", pointLightComponent->color), 
    _intensitySlider("Intensity", pointLightComponent->intensity, 0.0f, 20.0f), 
    _rangeSlider("Range", pointLightComponent->range, 0.0f, 100.0f),
    _innerCutoffSlider("Inner Cutoff Angle", glm::degrees(pointLightComponent->innerCutoff), 0.0, 88.9f),
    _outerCutoffSlider("Outer Cutoff Angle", glm::degrees(pointLightComponent->outerCutoff), 0.0, 89.0f)
{
  _colorSelector.setUpdateCallback([this](const glm::vec3& color) {
    _spotLightComponent->color = color;
    });
  _intensitySlider.setUpdateCallback([this](float intensity) {
    _spotLightComponent->intensity = intensity;
  });
  _rangeSlider.setUpdateCallback([this](float range) {
    _spotLightComponent->range = range;
  });
  _innerCutoffSlider.setUpdateCallback([this](float innerCutoffInDegrees) {
    _spotLightComponent->innerCutoff = std::min(glm::radians(innerCutoffInDegrees), 0.999f * _spotLightComponent->outerCutoff);
  });
  _outerCutoffSlider.setUpdateCallback([this](float outerCutoffInDegrees) {
    _spotLightComponent->outerCutoff = std::max(glm::radians(outerCutoffInDegrees), 1.001f * _spotLightComponent->innerCutoff);
  });
}

void SpotLightComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Spot Light", ImGuiTreeNodeFlags_DefaultOpen)) {
    _colorSelector.draw();
    _intensitySlider.draw();
    _rangeSlider.draw();
    _innerCutoffSlider.draw();
    _outerCutoffSlider.draw();

    ImGui::TreePop();
  }
}

}
