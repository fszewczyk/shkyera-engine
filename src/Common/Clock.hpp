#pragma once

#include <chrono>
#include <limits>

namespace shkyera {

template <size_t MaxNanosecondCount = std::numeric_limits<size_t>::max()>
struct Clock {
  float deltaTime{1.0 / 60.0};
  float scaleTime{0.0f};

  void reset() {
    const auto curTime = std::chrono::high_resolution_clock::now();
    const auto nanosecondDiff = std::min(
        MaxNanosecondCount,
        static_cast<size_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - _lastReset).count()));

    deltaTime = scaleTime * static_cast<float>(nanosecondDiff) * 1e-9;
    _lastReset = curTime;
  }

  bool isPaused() { return scaleTime == 0.0f; }

  void pause() { scaleTime = 0.0f; }

  void start(float speed = 1.0f) { scaleTime = speed; }

 private:
  std::chrono::high_resolution_clock::time_point _lastReset{};
};

namespace clock {
extern Clock<static_cast<size_t>(1e9 / 30)> Game;
}

}  // namespace shkyera
