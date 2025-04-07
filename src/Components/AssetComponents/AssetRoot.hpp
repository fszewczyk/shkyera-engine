#pragma once

#include <filesystem>

#include <ECS/Entity.hpp>
#include <ECS/SingletonComponent.hpp>

namespace shkyera {

// Registry of all path-based assets representing the root of the project
struct AssetRoot : public SingletonComponent {
  std::filesystem::path path;
};

}  // namespace shkyera
