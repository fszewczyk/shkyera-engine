#include "UI/ComponentUI.hpp"
#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/AudioSourceComponentUI.hpp>
#include <optional>

namespace shkyera {

AudioSourceComponentUI::AudioSourceComponentUI(std::shared_ptr<Registry> registry,
                                               AudioSourceComponent* audioSourceComponent)
    : ComponentUI("resources/icons/components/audio_source.png"),
      _registry(registry),
      _audioSourceComponent(audioSourceComponent),
      _audioSelector("Audio", registry.get(), std::get<OptionalAssetHandle>(audioSourceComponent->audio)),
      _pausedSelector("Paused", audioSourceComponent->paused),
      _loopSelector("Loop", audioSourceComponent->loop),
      _spatializeSelector("Spatialize", audioSourceComponent->spatialize),
      _volumeSlider("Volume", audioSourceComponent->volume, 0.0f, 2.0f),
      _maxDistanceSlider("Max Distance", audioSourceComponent->maxDistance, 1.5f, 100.0f) {
  _audioSelector.setUpdateCallback([this](const auto& assetHandle) {
    if (_registry->hasComponent<AssetComponent<Audio>>(assetHandle)) {
      auto& audioAsset = _registry->getComponent<AssetComponent<Audio>>(assetHandle);
      _audioSourceComponent->sound.reset();
      _audioSourceComponent->audio = HandleAndAsset<Audio>{assetHandle, utils::assets::read(audioAsset)};
    }
  });

  _audioSelector.setClearCallback([this] {
    _audioSourceComponent->sound.reset();
    _audioSourceComponent->audio = HandleAndAsset<Audio>(std::nullopt, nullptr);
  });

  _pausedSelector.setUpdateCallback([this](bool value) { _audioSourceComponent->paused = value; });

  _loopSelector.setUpdateCallback([this](bool value) { _audioSourceComponent->loop = value; });

  _spatializeSelector.setUpdateCallback([this](bool value) { _audioSourceComponent->spatialize = value; });

  _volumeSlider.setUpdateCallback([this](float value) { _audioSourceComponent->volume = value; });

  _maxDistanceSlider.setUpdateCallback([this](float value) { _audioSourceComponent->maxDistance = value; });
}

void AudioSourceComponentUI::draw() {
  ImGui::Image(_icon->getImguiTextureID(), ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Audio Source", ImGuiTreeNodeFlags_DefaultOpen)) {
    _audioSelector.draw();
    _pausedSelector.draw();
    _loopSelector.draw();
    _volumeSlider.draw();

    _spatializeSelector.draw();
    if (_audioSourceComponent->spatialize) {
      _maxDistanceSlider.draw();
    }
    ImGui::TreePop();
  }
}

}  // namespace shkyera
