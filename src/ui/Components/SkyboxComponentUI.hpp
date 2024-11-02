#pragma once

#include <UI/ComponentUI.hpp>
#include <Components/SkyboxComponent.hpp>

namespace shkyera {

class SkyboxComponentUI : public ComponentUI {
    public:
        SkyboxComponentUI(SkyboxComponent* directionalLightComponent);

        void draw() override;

    private:
        SkyboxComponent* _skyboxComponent;
};

}
