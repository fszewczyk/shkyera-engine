#pragma once

#include <Components/ParticleEmitterComponent.hpp>
#include <UI/Common/AssetSelector.hpp>
#include <UI/Common/BooleanSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class ParticleEmitterComponentUI : public ComponentUI {
   public:
    ParticleEmitterComponentUI(std::shared_ptr<Registry> registry, ParticleEmitterComponent* particleEmitterComponent);

    void draw() override;

   private:
    std::shared_ptr<Registry> _registry;
    ParticleEmitterComponent* _particleEmitterComponent;

    BooleanSelector _enabledSelector;

    FloatSlider _gravitySlider;

    FloatSlider _emittedAtRadiusSlider;
    FloatSlider _emittedPerSecondSlider;
    FloatSlider _emittedPerSecondVarianceSlider;

    FloatSlider _initialVelocitySlider;
    FloatSlider _initialVelocityVarianceSlider;
    FloatSlider _initialVelocityDispersionSlider;

    FloatSlider _lifetimeSlider;
    FloatSlider _lifetimeVarianceSlider;

    FloatSlider _initialParticleSizeSlider;
    FloatSlider _initialParticleSizeVarianceSlider;
    FloatSlider _endParticleSizeSlider;
    FloatSlider _endParticleSizeVarianceSlider;

    AssetSelector<Material> _materialSelector;
    FloatSlider _initialTransparencySlider;
    FloatSlider _initialTransparencyVarianceSlider;
    FloatSlider _endTransparencySlider;
    FloatSlider _endTransparencyVarianceSlider;
};

}  // namespace shkyera
