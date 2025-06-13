#pragma once

#include <set>
#include <string_view>

namespace shkyera {

using TypeID = uint64_t;

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
  TypeInfo() = delete;

  inline static constexpr TypeID ID = getId<T>();
};

using TypeSet = std::set<TypeID>;

}  // namespace shkyera
