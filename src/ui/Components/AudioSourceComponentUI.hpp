#pragma once

#include <AssetManager/Audio.hpp>
#include <Components/AudioSourceComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/Common/AssetSelector.hpp>
#include <UI/Common/BooleanSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class AudioSourceComponentUI : public ComponentUI {
 public:
  AudioSourceComponentUI(std::shared_ptr<Registry> registry, AudioSourceComponent* audioSourceComponent);

  void draw() override;

 private:
  AudioSourceComponent* _audioSourceComponent;
  std::shared_ptr<Registry> _registry;

  AssetSelector<Audio> _audioSelector;
  BooleanSelector _pausedSelector;
  BooleanSelector _loopSelector;
  BooleanSelector _spatializeSelector;
  FloatSlider _volumeSlider;
  FloatSlider _maxDistanceSlider;
};

}  // namespace shkyera 