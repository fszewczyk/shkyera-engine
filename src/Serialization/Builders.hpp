#pragma once

#include <filesystem>
#include <iostream>

#include <ECS/Registry.hpp>

namespace shkyera::serialization {

std::unique_ptr<Registry> fromBinary(std::istream& inputStream);

void toBinary(std::ostream& outputStream, Registry const* registry);

std::string pathAssetToBinary(const std::filesystem::path& path);

}  // namespace shkyera::serialization
