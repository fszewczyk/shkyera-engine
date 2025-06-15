#pragma once

#include <AssetManager/Asset.hpp>
#include <AssetManager/Audio.hpp>
#include <AssetManager/Sound.hpp>
#include <Components/BaseComponent.hpp>

#include <memory>

namespace shkyera {

/// Component that enables audio playback for an entity
class AudioSourceComponent : public BaseComponent<AudioSourceComponent> {
 public:
  /// Audio asset to play
  HandleAndAsset<Audio> audio{};
  /// Sound instance for playback control
  std::shared_ptr<Sound> sound{};

  bool paused{false};
  bool loop{false};
  bool spatialize{false};
  float volume{1.0};
  float maxDistance{10};
};

}  // namespace shkyera
