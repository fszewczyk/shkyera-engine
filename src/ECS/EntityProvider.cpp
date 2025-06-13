#include <ECS/EntityProvider.hpp>

namespace shkyera {

EntityProvider::EntityProvider() : _nextEntity(1) {}

EntityProvider::EntityProvider(const EntityProvider& provider) : _nextEntity(provider._nextEntity.load()) {}

EntityProvider& EntityProvider::operator=(const EntityProvider& other) {
  _nextEntity = other._nextEntity.load();
  return *this;
}

Entity EntityProvider::requestEntity() {
  return _nextEntity++;
}

}  // namespace shkyera
