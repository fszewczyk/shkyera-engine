#pragma once

#include <set>
#include <string_view>

namespace shkyera {

template <typename T>
constexpr uint64_t getId() {
  constexpr std::string_view name = __PRETTY_FUNCTION__;
  uint64_t hash = 0xcbf29ce484222325;
  for (char c : name) {
    hash ^= static_cast<uint64_t>(c);
    hash *= 0x100000001b3;
  }
  return hash;
}

template <typename T>
struct TypeInfo {
  using IDType = uint64_t;
  inline static constexpr IDType ID = getId<T>();
};

using TypeSet = std::set<TypeInfo<void>::IDType>;

}  // namespace shkyera
