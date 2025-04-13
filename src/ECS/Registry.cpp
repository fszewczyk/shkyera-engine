#include <Common/Profiler.hpp>
#include <ECS/Registry.hpp>

namespace shkyera {

Registry::Registry(const Registry& other)
    : _entityProvider(other._entityProvider),
      _entityHierarchy(other._entityHierarchy),
      _selectedEntities(other._selectedEntities) {
  SHKYERA_PROFILE("Registry Copy Construct");
  for (const auto& [typeId, setPtr] : other._componentSets) {
    _componentSets[typeId] = setPtr->clone();
  }
}

Registry& Registry::operator=(const Registry& other) {
  SHKYERA_PROFILE("Registry Copy Assignment");
  if (this != &other) {
    _entityProvider = other._entityProvider;
    _entityHierarchy = other._entityHierarchy;
    _selectedEntities = other._selectedEntities;

    _componentSets.clear();
    for (const auto& [typeId, setPtr] : other._componentSets) {
      _componentSets[typeId] = setPtr->clone();
    }
  }
  return *this;
}

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
