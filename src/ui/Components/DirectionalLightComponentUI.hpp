#pragma once

#include <UI/ComponentUI.hpp>
#include <UI/Common/ColorSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <Components/DirectionalLightComponent.hpp>

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

}
