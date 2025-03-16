#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/PointLightComponentUI.hpp>

namespace shkyera {

PointLightComponentUI::PointLightComponentUI(PointLightComponent* pointLightComponent)
    : ComponentUI("resources/icons/components/point_light.png"),
      _pointLightComponent(pointLightComponent),
      _colorSelector("Color", pointLightComponent->color),
      _intensitySlider("Intensity", pointLightComponent->intensity, 0.0f, 20.0f),
      _rangeSlider("Range", pointLightComponent->range, 0.0f, 100.0f) {
    _colorSelector.setUpdateCallback([this](const glm::vec3& color) {
        _pointLightComponent->color = color;
    });
    _intensitySlider.setUpdateCallback([this](float intensity) {
        _pointLightComponent->intensity = intensity;
    });
    _rangeSlider.setUpdateCallback([this](float range) {
        _pointLightComponent->range = range;
    });
}

void PointLightComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
                 ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNodeEx("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
        _colorSelector.draw();
        _intensitySlider.draw();
        _rangeSlider.draw();

        ImGui::TreePop();
    }
}

}  // namespace shkyera
