#pragma once

#include <UI/ComponentUI.hpp>
#include <UI/Common/ColorSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <Components/SpotLightComponent.hpp>

namespace shkyera {

class SpotLightComponentUI : public ComponentUI {
    public:
        SpotLightComponentUI(SpotLightComponent* pointLightComponent);

        void draw() override;

    private:
        ColorSelector _colorSelector;
        FloatSlider _intensitySlider;
        FloatSlider _rangeSlider;
        FloatSlider _innerCutoffSlider;
        FloatSlider _outerCutoffSlider;
        SpotLightComponent* _spotLightComponent;
};

}
