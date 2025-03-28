#include <ECS/EntityProvider.hpp>

namespace shkyera {

EntityProvider::EntityProvider(Entity numEntities) : _numEntities(0) {
  allocateEntities(numEntities);
}

Entity EntityProvider::requestEntity() {
  return ++_numEntities;
  
  if (_entities.size() == 0) {
    allocateEntities(_numEntities);
  }

  const auto& entity = _entities.front();
  _entities.pop();
  return entity;
}

void EntityProvider::removeEntity(Entity entity) {
  _entities.emplace(entity);
}

void EntityProvider::allocateEntities(Entity entitiesToAllocate) {
  for (Entity e = _numEntities; e < _numEntities + entitiesToAllocate; ++e) {
    _entities.emplace(e);
  }

  _numEntities += entitiesToAllocate;
}

}  // namespace shkyera
