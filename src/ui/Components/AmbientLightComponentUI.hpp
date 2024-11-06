#pragma once

#include <UI/ComponentUI.hpp>
#include <UI/Common/ColorSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <Components/AmbientLightComponent.hpp>

namespace shkyera {

class AmbientLightComponentUI : public ComponentUI {
    public:
        AmbientLightComponentUI(AmbientLightComponent* ambientLightComponent);

        void draw() override;

    private:
        ColorSelector _colorSelector;
        FloatSlider _intensitySlider;
        AmbientLightComponent* _ambientLightComponent;
};

}
