#pragma once

namespace shkyera {

template <typename T>
struct ThreadRequirements {
  static constexpr bool OnlyMainThread = false;
};

template <typename T>
constexpr bool OnlyMainThread = false;

}  // namespace shkyera
