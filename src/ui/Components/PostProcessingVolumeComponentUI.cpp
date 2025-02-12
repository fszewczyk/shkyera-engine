#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/PostProcessingVolumeComponentUI.hpp>

namespace shkyera {

PostProcessingVolumeComponentUI::PostProcessingVolumeComponentUI(PostProcessingVolumeComponent* postProcessingVolumeComponent) :
    ComponentUI("resources/icons/components/post_processing_colors.png"),
    _antiAliasingSelector("FXAA", postProcessingVolumeComponent->antiAliasing), 
    _globalSelector("Global", postProcessingVolumeComponent->global), 
    _bloomSelector("Bloom", postProcessingVolumeComponent->bloom), 
    _toneMappingSelector("HDR", postProcessingVolumeComponent->toneMapping), 
    _gammaSlider("Gamma Correction", postProcessingVolumeComponent->gamma, 0.5f, 2.0f), 
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
    _bloomSelector.setUpdateCallback([this](bool enabled) {
      _postProcessingVolumeComponent->bloom = enabled;
    });
    _gammaSlider.setUpdateCallback([this](float gamma) {
      _postProcessingVolumeComponent->gamma = gamma;
    });
}

void PostProcessingVolumeComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
               ImVec2(16, 16));
    ImGui::SameLine();
  if (ImGui::TreeNodeEx("Post-Processing Volume", ImGuiTreeNodeFlags_DefaultOpen)) {
    _globalSelector.draw();
    _toneMappingSelector.draw();
    _bloomSelector.draw();
    _antiAliasingSelector.draw();
    _gammaSlider.draw();

    ImGui::TreePop();
  }
}

}
