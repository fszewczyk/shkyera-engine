#pragma once

#include <UI/ComponentUI.hpp>
#include <Components/AmbientLightComponent.hpp>

namespace shkyera {

class AmbientLightComponentUI : public ComponentUI {
    public:
        AmbientLightComponentUI(AmbientLightComponent* ambientLightComponent);

        void draw() override;

    private:
        AmbientLightComponent* _ambientLightComponent;
};

}
