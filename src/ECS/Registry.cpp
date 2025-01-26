#include <ECS/Registry.hpp>

namespace shkyera {

static constexpr Entity INITIAL_ENTITIES = 100;

Registry::Registry() : _entityProvider(INITIAL_ENTITIES) {
  _camera = addEntity();
  _environment = addEntity();
}

Entity Registry::addEntity() {
  return _entityProvider.requestEntity();
}

void Registry::removeEntity(Entity entity) {
  for (auto& [typeId, componentSet] : _componentSets) {
    componentSet->remove(entity);
  }

  _entityProvider.removeEntity(entity);
}

const std::unordered_set<Entity>& Registry::getSelectedEntities()
{
    return _selectedEntities;
}

void Registry::selectEntity(Entity entity)
{
    _selectedEntities.insert(entity);
}

void Registry::clearSelectedEntities()
{
    _selectedEntities.clear();
}

Entity Registry::getCamera() const {
  return _camera;
}

Entity Registry::getEnvironment() const {
  return _environment;
}

EntityHierarchy& Registry::getHierarchy() {
  return _entityHierarchy;
}

const EntityHierarchy& Registry::getHierarchy() const {
  return _entityHierarchy;
}

}  // namespace shkyera
