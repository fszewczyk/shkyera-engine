#include <ECS/Registry.hpp>

namespace shkyera
{

static constexpr Entity InitialEntities = 100; 

Registry::Registry() : _entityProvider(InitialEntities) {}

Entity Registry::addEntity()
{
    return _entityProvider.requestEntity();
}

void Registry::removeEntity(Entity entity)
{
    for(auto &[typeId, componentSet] : _componentSets)
    {
        componentSet->remove(entity);
    }

    _entityProvider.removeEntity(entity);
}

}
