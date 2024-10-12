#include <ECS/Registry.hpp>

namespace shkyera
{

static constexpr Entity InitialEntities = 1000; 

Registry::Registry() : _entityProvider(InitialEntities) {}

Entity Registry::addEntity()
{
    return _entityProvider.requestEntity();
}

void Registry::removeEntity(Entity entity)
{
    _entityProvider.removeEntity(entity);
}

}
