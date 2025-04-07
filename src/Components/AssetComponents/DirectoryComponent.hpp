#pragma once

#include <filesystem>
#include <map>

#include <AssetManager/Asset.hpp>

namespace shkyera {

struct DirectoryComponent {
  explicit DirectoryComponent(std::filesystem::path path) : name(path.stem()) {}

  explicit DirectoryComponent(std::string name_) : name(std::move(name_)) {}

  std::string name;
};

}  // namespace shkyera
