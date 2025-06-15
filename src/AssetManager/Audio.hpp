#pragma once

#include <filesystem>
#include <memory>

// Forward declaration of miniaudio decoder
class ma_decoder;

namespace shkyera {

/// Audio asset that manages a single audio file
class Audio {
 public:
  /// @param path Path to the audio file
  explicit Audio(const std::filesystem::path& path);
  ~Audio();

  // Delete copy operations
  Audio(const Audio&) = delete;
  Audio& operator=(const Audio&) = delete;

  // Add move operations
  Audio(Audio&& other) noexcept;
  Audio& operator=(Audio&& other) noexcept;

  /// @return Pointer to the miniaudio decoder for this audio file
  ma_decoder* getDecoder();

 private:
  /// Internal miniaudio decoder
  std::unique_ptr<ma_decoder> _decoder;
};

}  // namespace shkyera
