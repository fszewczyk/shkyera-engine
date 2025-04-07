#pragma once

#include <Components/PostProcessingVolumeComponent.hpp>
#include <UI/Common/BooleanSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class PostProcessingVolumeComponentUI : public ComponentUI {
 public:
  PostProcessingVolumeComponentUI(PostProcessingVolumeComponent* postProcessingVolumeComponent);

  void draw() override;

 private:
  BooleanSelector _globalSelector;
  BooleanSelector _toneMappingSelector;
  BooleanSelector _antiAliasingSelector;
  FloatSlider _gammaSlider;

  FloatSlider _bloomThresholdSlider;
  FloatSlider _bloomWeightSlider;

  FloatSlider _ssaoStrengthSlider;
  FloatSlider _ssaoRadiusSlider;

  PostProcessingVolumeComponent* _postProcessingVolumeComponent;
};

}  // namespace shkyera
