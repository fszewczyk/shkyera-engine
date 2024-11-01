#pragma once

#include <UI/ComponentUI.hpp>
#include <Components/DirectionalLightComponent.hpp>

namespace shkyera {

class DirectionalLightComponentUI : public ComponentUI {
    public:
        DirectionalLightComponentUI(DirectionalLightComponent* directionalLightComponent);

        void draw() override;

    private:
        DirectionalLightComponent* _directionalLightComponent;
};

}
