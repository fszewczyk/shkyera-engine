#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/ParticleEmitterComponentUI.hpp>

namespace shkyera {

ParticleEmitterComponentUI::ParticleEmitterComponentUI(std::shared_ptr<Registry> registry, ParticleEmitterComponent* particleEmitterComponent)
    : ComponentUI("resources/icons/components/particles.png"),
      _registry(registry),
      _particleEmitterComponent(particleEmitterComponent),
      _enabledSelector("Enabled", particleEmitterComponent->enabled),
      _gravitySlider("Gravity", particleEmitterComponent->gravity, -20.0f, 20.0f),
      _emittedAtRadiusSlider("Radius", particleEmitterComponent->emittedAtRadius, 0.0f, 1.0f),
      _emittedPerSecondSlider("Frequency", particleEmitterComponent->emittedPerSecond, 1.0f, 100.0f),
      _emittedPerSecondVarianceSlider("Frequency Variance", particleEmitterComponent->emittedPerSecondVariance, 0.0f, 1.0f),
      _initialVelocitySlider("Initial Velocity", particleEmitterComponent->initialVelocity, 0.0f, 5.0f),
      _initialVelocityVarianceSlider("Initial Velocity Variance", particleEmitterComponent->initialVelocity, 0.0f, 1.0f),
      _initialVelocityDispersionSlider("Velocity Disperion", particleEmitterComponent->initialVelocityDispersion, -1.0f, 1.0f),
      _lifetimeSlider("Lifetime", particleEmitterComponent->lifetime, 0.1f, 10.0f),
      _lifetimeVarianceSlider("Lifetime Variance", particleEmitterComponent->lifetimeVariance, 0.0f, 1.0f),
      _initialParticleSizeSlider("Start Size", particleEmitterComponent->initialParticleSize, 0.01f, 1.0f),
      _initialParticleSizeVarianceSlider("Start Size Variance", particleEmitterComponent->initialParticleSizeVariance, 0.0f, 1.0f),
      _endParticleSizeSlider("End Size", particleEmitterComponent->endParticleSize, 0.0f, 1.0f),
      _endParticleSizeVarianceSlider("End Size Variance", particleEmitterComponent->endParticleSizeVariance, 0.0f, 1.0f),
      _materialSelector("Material", registry.get(), std::get<OptionalAssetHandle>(particleEmitterComponent->material)),
      _initialTransparencySlider("Start Alpha", particleEmitterComponent->initialTransparency, 0.0f, 1.0f),
      _initialTransparencyVarianceSlider("Start Alpha Variance", particleEmitterComponent->initialTransparencyVariance, 0.0f, 1.0f),
      _endTransparencySlider("End Alpha", particleEmitterComponent->endTransparency, 0.0f, 1.0f),
      _endTransparencyVarianceSlider("End Alpha Variance", particleEmitterComponent->endTransparencyVariance, 0.0f, 1.0f) {
    _enabledSelector.setUpdateCallback([this](auto enabled) {
        _particleEmitterComponent->enabled = enabled;
    });

    _materialSelector.setClearCallback([this]() {
        _particleEmitterComponent->material = HandleAndAsset<Material>{std::nullopt, nullptr};
    });

    _materialSelector.setUpdateCallback([this](const auto& assetHandle) {
        if (_registry->hasComponent<AssetComponent<Material>>(assetHandle)) {
            auto& materialAsset = _registry->getComponent<AssetComponent<Material>>(assetHandle);
            _particleEmitterComponent->material = HandleAndAsset<Material>{assetHandle, utils::assets::read(materialAsset)};
        }
    });

    // Lambda to set update callbacks
    auto setSliderCallback = [this](auto& slider, auto& member) {
        slider.setUpdateCallback([this, &member](float value) {
            member = value;
        });
    };

    // Apply the lambda to all sliders
    setSliderCallback(_gravitySlider, _particleEmitterComponent->gravity);
    setSliderCallback(_emittedAtRadiusSlider, _particleEmitterComponent->emittedAtRadius);
    setSliderCallback(_emittedPerSecondSlider, _particleEmitterComponent->emittedPerSecond);
    setSliderCallback(_emittedPerSecondVarianceSlider, _particleEmitterComponent->emittedPerSecondVariance);
    setSliderCallback(_initialVelocitySlider, _particleEmitterComponent->initialVelocity);
    setSliderCallback(_initialVelocityVarianceSlider, _particleEmitterComponent->initialVelocityVariance);
    setSliderCallback(_initialVelocityDispersionSlider, _particleEmitterComponent->initialVelocityDispersion);
    setSliderCallback(_lifetimeSlider, _particleEmitterComponent->lifetime);
    setSliderCallback(_lifetimeVarianceSlider, _particleEmitterComponent->lifetimeVariance);
    setSliderCallback(_initialParticleSizeSlider, _particleEmitterComponent->initialParticleSize);
    setSliderCallback(_initialParticleSizeVarianceSlider, _particleEmitterComponent->initialParticleSizeVariance);
    setSliderCallback(_endParticleSizeSlider, _particleEmitterComponent->endParticleSize);
    setSliderCallback(_endParticleSizeVarianceSlider, _particleEmitterComponent->endParticleSizeVariance);
    setSliderCallback(_initialTransparencySlider, _particleEmitterComponent->initialTransparency);
    setSliderCallback(_initialTransparencyVarianceSlider, _particleEmitterComponent->initialTransparencyVariance);
    setSliderCallback(_endTransparencySlider, _particleEmitterComponent->endTransparency);
    setSliderCallback(_endTransparencyVarianceSlider, _particleEmitterComponent->endTransparencyVariance);
}

static void drawSectionTitle(std::string_view text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.data()).x;

    ImGui::Spacing();
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::PushFont(style::NORMAL_FONT);
    ImGui::TextUnformatted(text.data());
    ImGui::PopFont();
}

void ParticleEmitterComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
                 ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNodeEx("Particle Emitter", ImGuiTreeNodeFlags_DefaultOpen)) {
        _enabledSelector.draw();
        _gravitySlider.draw();

        drawSectionTitle("Emission");
        _emittedPerSecondSlider.draw();
        _emittedPerSecondVarianceSlider.draw();
        _emittedAtRadiusSlider.draw();

        drawSectionTitle("Material");
        _materialSelector.draw();
        _initialTransparencySlider.draw();
        _initialTransparencyVarianceSlider.draw();
        _endTransparencySlider.draw();
        _endTransparencyVarianceSlider.draw();

        drawSectionTitle("Initial Dynamics");
        _initialVelocitySlider.draw();
        _initialVelocityVarianceSlider.draw();
        _initialVelocityDispersionSlider.draw();

        drawSectionTitle("Lifetime");
        _lifetimeSlider.draw();
        _lifetimeVarianceSlider.draw();

        drawSectionTitle("Size");
        _initialParticleSizeSlider.draw();
        _initialParticleSizeVarianceSlider.draw();
        _endParticleSizeSlider.draw();
        _endParticleSizeVarianceSlider.draw();

        ImGui::TreePop();
    }
}

}  // namespace shkyera
