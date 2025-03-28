#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Common/Utils.hpp>
#include <UI/Components/PostProcessingVolumeComponentUI.hpp>

namespace shkyera {

PostProcessingVolumeComponentUI::PostProcessingVolumeComponentUI(PostProcessingVolumeComponent* postProcessingVolumeComponent) :
    ComponentUI("resources/icons/components/post_processing_colors.png"),
    _antiAliasingSelector("FXAA", postProcessingVolumeComponent->antiAliasing), 
    _globalSelector("Global", postProcessingVolumeComponent->global), 
    _bloomThresholdSlider("Bloom Threshold", postProcessingVolumeComponent->bloomThreshold, 0.5f, 10.0f), 
    _bloomWeightSlider("Bloom Weight", postProcessingVolumeComponent->bloomWeight, 0.0f, 1.0f), 
    _toneMappingSelector("HDR", postProcessingVolumeComponent->toneMapping), 
    _gammaSlider("Gamma Correction", postProcessingVolumeComponent->gamma, 0.5f, 2.0f), 
    _ssaoStrengthSlider("SSAO Strength", postProcessingVolumeComponent->ssaoStrength, 0.01f, 2.0f), 
    _ssaoRadiusSlider("SSAO Radius", postProcessingVolumeComponent->ssaoRadius, 0.1f, 5.0f), 
    _postProcessingVolumeComponent(postProcessingVolumeComponent) 
{
    _antiAliasingSelector.setUpdateCallback([this](bool enabled) {
      _postProcessingVolumeComponent->antiAliasing = enabled;
    });
    _toneMappingSelector.setUpdateCallback([this](bool enabled) {
      _postProcessingVolumeComponent->toneMapping = enabled;
    });
    _globalSelector.setUpdateCallback([this](bool enabled) {
      _postProcessingVolumeComponent->global = enabled;
    });
    _bloomThresholdSlider.setUpdateCallback([this](float threshold) {
      _postProcessingVolumeComponent->bloomThreshold = threshold;
    });
    _bloomWeightSlider.setUpdateCallback([this](float weight) {
      _postProcessingVolumeComponent->bloomWeight = weight;
    });
    _gammaSlider.setUpdateCallback([this](float gamma) {
      _postProcessingVolumeComponent->gamma = gamma;
    });
    _ssaoStrengthSlider.setUpdateCallback([this](float strength) {
      _postProcessingVolumeComponent->ssaoStrength = strength;
    });
    _ssaoRadiusSlider.setUpdateCallback([this](float radius) {
      _postProcessingVolumeComponent->ssaoRadius = radius;
    });
}

void PostProcessingVolumeComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Post-Processing Volume", ImGuiTreeNodeFlags_DefaultOpen)) {
    _globalSelector.draw();

    ImGui::Separator();

    utils::ui::drawCenteredTitle("Bloom", style::NORMAL_FONT);
    _bloomWeightSlider.draw();
    _bloomThresholdSlider.draw();

    ImGui::Separator();

    utils::ui::drawCenteredTitle("SSAO", style::NORMAL_FONT);
    _ssaoStrengthSlider.draw();
    _ssaoRadiusSlider.draw();

    ImGui::Separator();

    utils::ui::drawCenteredTitle("Color Grading", style::NORMAL_FONT);
    _toneMappingSelector.draw();
    _gammaSlider.draw();

    ImGui::Separator();

    utils::ui::drawCenteredTitle("Image Quality", style::NORMAL_FONT);
    _antiAliasingSelector.draw();

    ImGui::TreePop();
  }
}

}
