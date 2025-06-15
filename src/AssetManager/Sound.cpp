#include <AssetManager/Sound.hpp>
#include <Common/Assert.hpp>

#include <miniaudio.h>

#include <memory>

namespace shkyera {

Sound::Sound(std::unique_ptr<ma_sound> sound_) : sound(std::move(sound_)) {
  SHKYERA_ASSERT(sound, "Provided sound is nullptr");
}

Sound::~Sound() {
  if (sound) {
    ma_sound_uninit(sound.get());
  }
}

}  // namespace shkyera
