#pragma once

#include <UI/ComponentUI.hpp>
#include <UI/Common/BooleanSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <Components/PostProcessingVolumeComponent.hpp>

namespace shkyera {

class PostProcessingVolumeComponentUI : public ComponentUI {
    public:
        PostProcessingVolumeComponentUI(PostProcessingVolumeComponent* postProcessingVolumeComponent);

        void draw() override;

    private:
        BooleanSelector _globalSelector;
        BooleanSelector _toneMappingSelector;
        BooleanSelector _antiAliasingSelector;
        BooleanSelector _bloomSelector;
        FloatSlider _gammaSlider;
        
        PostProcessingVolumeComponent* _postProcessingVolumeComponent;
};

}
