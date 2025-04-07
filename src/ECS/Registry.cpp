#include <ECS/Registry.hpp>

namespace shkyera {

Entity Registry::addEntity() {
  return _entityProvider.requestEntity();
}

void Registry::removeEntity(Entity entity) {
  for (auto& [typeId, componentSet] : _componentSets) {
    componentSet->remove(entity);
  }
}

const std::unordered_set<Entity>& Registry::getSelectedEntities() {
  return _selectedEntities;
}

void Registry::selectEntity(Entity entity) {
  _selectedEntities.insert(entity);
}

void Registry::clearSelectedEntities() {
  _selectedEntities.clear();
}

EntityHierarchy& Registry::getHierarchy() {
  return _entityHierarchy;
}

const EntityHierarchy& Registry::getHierarchy() const {
  return _entityHierarchy;
}

}  // namespace shkyera
