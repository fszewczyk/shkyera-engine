#pragma once

#include <Common/TypeInfo.hpp>

namespace shkyera {

struct ResourceTag {};

template <typename T>
constexpr bool OnlyMainThread = false;

}  // namespace shkyera
