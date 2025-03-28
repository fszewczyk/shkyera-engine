#pragma once

#include <iostream>

#include <ECS/Registry.hpp>

namespace shkyera::serialization {

std::unique_ptr<shkyera::Registry> fromBinary(std::istream& inputStream);

void toBinary(std::ostream& outputStream, Registry const* registry);

}  // namespace shkyera::serialization
