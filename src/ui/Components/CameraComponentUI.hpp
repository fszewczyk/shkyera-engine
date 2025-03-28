#pragma once

#include <UI/ComponentUI.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/Common/EnumSelector.hpp>
#include <Components/CameraComponent.hpp>

namespace shkyera {

class CameraComponentUI : public ComponentUI {
    public:
        CameraComponentUI(CameraComponent* cameraComponent);

        void draw() override;

    private:
        FloatSlider _fovSlider;
        EnumSelector<CameraComponent::ProjectionType> _projectionSelector;
        CameraComponent* _cameraComponent;
};

}
