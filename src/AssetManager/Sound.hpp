#pragma once

#include <memory>

class ma_sound;

namespace shkyera {

/// Sound instance that manages playback of an audio asset
struct Sound {
 public:
  /// @param sound Raw miniaudio sound instance to manage
  explicit Sound(std::unique_ptr<ma_sound> sound);
  ~Sound();

  /// Raw miniaudio sound instance
  std::unique_ptr<ma_sound> sound;
};

}  // namespace shkyera
