#pragma once

#include <Components/DirectionalLightComponent.hpp>
#include <UI/Common/ColorSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class DirectionalLightComponentUI : public ComponentUI {
   public:
    DirectionalLightComponentUI(DirectionalLightComponent* directionalLightComponent);

    void draw() override;

   private:
    ColorSelector _colorSelector;
    FloatSlider _intensitySlider;
    DirectionalLightComponent* _directionalLightComponent;
};

}  // namespace shkyera
