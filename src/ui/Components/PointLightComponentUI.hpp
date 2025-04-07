#pragma once

#include <Components/PointLightComponent.hpp>
#include <UI/Common/ColorSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class PointLightComponentUI : public ComponentUI {
 public:
  PointLightComponentUI(PointLightComponent* pointLightComponent);

  void draw() override;

 private:
  ColorSelector _colorSelector;
  FloatSlider _intensitySlider;
  FloatSlider _rangeSlider;
  PointLightComponent* _pointLightComponent;
};

}  // namespace shkyera
