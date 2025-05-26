#pragma once

#include <type_traits>

namespace shkyera {

/// @brief Marker to denote Singleton Components
struct SingletonComponent {};

template <typename Component>
concept SingletonComponentType = std::is_base_of_v<SingletonComponent, Component>;

template <typename Component>
concept NonSingletonComponentType = !SingletonComponentType<Component>;

}  // namespace shkyera
