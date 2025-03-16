#pragma once

#include <Components/CameraComponent.hpp>
#include <UI/Common/EnumSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/ComponentUI.hpp>

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

}  // namespace shkyera
