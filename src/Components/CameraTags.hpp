#pragma once

#include <ECS/SingletonComponent.hpp>

namespace shkyera {

struct SceneCamera : public SingletonComponent {};
struct RuntimeCamera : public SingletonComponent {};

}  // namespace shkyera
