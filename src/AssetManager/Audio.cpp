#include <AssetManager/Audio.hpp>
#include <Common/Assert.hpp>

#include <filesystem>
#include <memory>

#include <miniaudio.h>

namespace shkyera {

Audio::Audio(const std::filesystem::path& path) {
  _decoder = std::make_unique<ma_decoder>();
  if (ma_decoder_init_file(path.string().c_str(), nullptr, _decoder.get()) != MA_SUCCESS) {
    SHKYERA_ASSERT(false, "Failed to load an Audio from path {}", path.string());
    _decoder = nullptr;
  }
}

Audio::~Audio() {
  if (_decoder) {
    ma_decoder_uninit(_decoder.get());
  }
}

Audio::Audio(Audio&& other) noexcept : _decoder(std::move(other._decoder)) {
}

Audio& Audio::operator=(Audio&& other) noexcept {
  if (this != &other) {
    if (_decoder) {
      ma_decoder_uninit(_decoder.get());
    }
    _decoder = std::move(other._decoder);
  }
  return *this;
}

ma_decoder* Audio::getDecoder() {
  return _decoder.get();
}

}  // namespace shkyera
