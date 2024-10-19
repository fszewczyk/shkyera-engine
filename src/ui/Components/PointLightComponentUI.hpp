#pragma once

#include <UI/ComponentUI.hpp>
#include <Components/PointLightComponent.hpp>

namespace shkyera {

class PointLightComponentUI : public ComponentUI {
    public:
        PointLightComponentUI(PointLightComponent* pointLightComponent);

        void draw() override;

    private:
        PointLightComponent* _pointLightComponent;
};

}
