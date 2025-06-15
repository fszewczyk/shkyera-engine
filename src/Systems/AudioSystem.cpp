#include <Common/Assert.hpp>
#include <Common/Clock.hpp>
#include <Components/AudioSourceComponent.hpp>
#include <Components/RenderingTextureComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Systems/AudioSystem.hpp>
#include <Utils/TransformUtils.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include "Common/Profiler.hpp"

#include <miniaudio.h>

namespace shkyera {

AudioSystem::AudioSystem(std::shared_ptr<Registry> registry)
    : RegistryViewer(std::move(registry), ReadAccess<TransformComponent, EntityHierarchy, RuntimeCamera>(),
                     WriteAccess<AudioSourceComponent>()) {
  _engine = std::make_unique<ma_engine>();
  if (ma_engine_init(nullptr, _engine.get()) != MA_SUCCESS) {
    _engine = nullptr;
    SHKYERA_ASSERT(false, "MiniAudio Engine failed to initialize.");
  }
}

AudioSystem::~AudioSystem() {
  if (_engine) {
    ma_engine_uninit(_engine.get());
  }
}

void AudioSystem::update() {
  SHKYERA_PROFILE("AudioSystem::update");

  for (auto& audioSource : RegistryViewer::write<AudioSourceComponent>()) {
    const auto loaded = ensureLoaded(audioSource);
    SHKYERA_ASSERT(!std::get<AssetRef<Audio>>(audioSource.audio) || loaded, "Sound is not properly loaded");

    if (loaded) {
      handlePlayback(audioSource);
    }
  }
  handleVolume();
  handleSpatiality();
}

bool AudioSystem::ensureLoaded(AudioSourceComponent& audioSource) {
  if (!audioSource.sound) {
    if (auto* audioAsset = std::get<AssetRef<Audio>>(audioSource.audio).get()) {
      auto masound = std::make_unique<ma_sound>();
      const auto loadResult =
          ma_sound_init_from_data_source(_engine.get(), audioAsset->getDecoder(), 0, nullptr, masound.get());
      SHKYERA_ASSERT(loadResult == MA_SUCCESS, "Sound has not beem loaded properly");
      SHKYERA_ASSERT(masound != nullptr, "Sound has not beem loaded properly");
      audioSource.sound = std::make_shared<Sound>(std::move(masound));
    }
  }

  return audioSource.sound != nullptr;
}

void AudioSystem::handleVolume() {
  if (clock::Game.isPaused()) {
    return;
  }

  const auto cameraEntity = RegistryViewer::getEntity<RuntimeCamera>();
  if (!cameraEntity) {
    SHKYERA_ASSERT(false, "RuntimeCamera is not assigned to any entity");
    return;
  }

  const auto& cameraTransformMatrix = utils::transform::getGlobalTransformMatrix(*cameraEntity, *this);
  const auto& listenerPosition = glm::vec3{cameraTransformMatrix[3]};

  for (const auto& [entity, audioSource] : RegistryViewer::writeAll<AudioSourceComponent>()) {
    if (!audioSource.sound || !audioSource.sound->sound) {
      continue;
    }

    float finalVolume = audioSource.volume;

    if (audioSource.spatialize) {
      const auto& sourceTransformMatrix = utils::transform::getGlobalTransformMatrix(entity, *this);
      const auto& sourcePosition = glm::vec3{sourceTransformMatrix[3]};
      const float distance = glm::distance(listenerPosition, sourcePosition);

      constexpr auto ReferenceDistance = 1.0f;
      if (audioSource.maxDistance > ReferenceDistance) {
        if (distance >= audioSource.maxDistance) {
          finalVolume = 0.0f;
        } else {
          const float decayRate = 5.0f / (audioSource.maxDistance - ReferenceDistance);
          float decay = std::exp(-decayRate * (distance - ReferenceDistance));
          finalVolume = std::min(audioSource.volume, audioSource.volume * decay);
        }
      }
    }

    ma_sound_set_volume(audioSource.sound->sound.get(), finalVolume);
  }
}

void AudioSystem::handlePlayback(AudioSourceComponent& audioSource) {
  if (!audioSource.sound || !audioSource.sound->sound) {
    return;
  }

  const auto isPlaying = ma_sound_is_playing(audioSource.sound->sound.get());
  const auto isGameRunning = !clock::Game.isPaused();
  const auto isPaused = audioSource.paused;

  ma_sound_set_looping(audioSource.sound->sound.get(), audioSource.loop);

  const auto gameJustStopped = !isGameRunning && isPlaying;
  if (gameJustStopped) {
    ma_sound_seek_to_pcm_frame(audioSource.sound->sound.get(), 0);
  }

  if (isGameRunning && !isPaused && !isPlaying) {
    ma_sound_start(audioSource.sound->sound.get());
  } else if (isPaused && isPlaying) {
    ma_sound_stop(audioSource.sound->sound.get());
  }
}

void AudioSystem::handleSpatiality() {
  if (clock::Game.isPaused()) {
    return;
  }

  const auto cameraEntity = RegistryViewer::getEntity<RuntimeCamera>();
  if (!cameraEntity) {
    SHKYERA_ASSERT(false, "RuntimeCamera is not assigned to any entity");
    return;
  }

  const auto& cameraTransformMatrix = utils::transform::getGlobalTransformMatrix(*cameraEntity, *this);
  const auto& listenerPosition = glm::vec3{cameraTransformMatrix[3]};
  const auto& listenerForward = glm::vec3{cameraTransformMatrix[0]};

  // For whatever reason, z needs to be negative
  ma_engine_listener_set_position(_engine.get(), 0, listenerPosition.x, listenerPosition.y, -listenerPosition.z);
  ma_engine_listener_set_direction(_engine.get(), 0, listenerForward.x, listenerForward.y, listenerForward.z);

  for (const auto& [entity, audioSource] : RegistryViewer::writeAll<AudioSourceComponent>()) {
    if (!audioSource.sound || !audioSource.sound->sound) {
      continue;
    }

    if (audioSource.spatialize) {
      ma_sound_set_spatialization_enabled(audioSource.sound->sound.get(), MA_TRUE);

      const auto& sourceTransformMatrix = utils::transform::getGlobalTransformMatrix(entity, *this);
      const auto& sourcePosition = glm::vec3{sourceTransformMatrix[3]};

      ma_sound_set_position(audioSource.sound->sound.get(), sourcePosition.x, sourcePosition.y, -sourcePosition.z);
    } else {
      ma_sound_set_spatialization_enabled(audioSource.sound->sound.get(), MA_FALSE);
    }
  }
}

}  // namespace shkyera
