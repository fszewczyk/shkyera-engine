#pragma once

#include <Components/AudioSourceComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <ECS/RegistryViewer.hpp>

#include <memory>

class ma_engine;

namespace shkyera {

/// System that manages audio playback and spatial audio
class AudioSystem : public RegistryViewer {
 public:
  /// @param registry Registry to read audio components from
  explicit AudioSystem(std::shared_ptr<Registry> registry);
  ~AudioSystem();

  /// Updates audio playback state and spatial audio
  void update();

 private:
  /// @param audioSource Audio source to check
  /// @return Whether the audio source is properly loaded
  bool ensureLoaded(AudioSourceComponent& audioSource);

  /// @param audioSource Audio source to update playback for
  void handlePlayback(AudioSourceComponent& audioSource);

  /// Updates volume for all audio sources
  void handleVolume();

  /// Updates spatial audio based on camera position
  void handleSpatiality();

  /// Miniaudio engine instance
  std::unique_ptr<ma_engine> _engine;
};

}  // namespace shkyera
