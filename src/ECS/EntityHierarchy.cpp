#include <ECS/EntityHierarchy.hpp>

namespace shkyera {

void EntityHierarchy::attributeChild(Entity parent, Entity child)
{
    if(isAncestor(parent, child))
    {
        return;
    }

    const auto& existingParentIt = _childToParent.find(child);
    if(existingParentIt != _childToParent.end())
    {
        _parentToChildren.at(existingParentIt->second).erase(child);
    }
    _parentToChildren[parent].insert(child);
    _childToParent[child] = parent;
}

void EntityHierarchy::removeFromParent(Entity child)
{
    const auto& parentIt = _childToParent.find(child);
    if(parentIt != _childToParent.end())
    {
        _parentToChildren.at(parentIt->second).erase(child);
        _childToParent.erase(child);
    }
}

std::optional<Entity> EntityHierarchy::getParent(Entity child) const
{
    const auto& parentIt = _childToParent.find(child);
    if(parentIt == _childToParent.end())
    {
        return std::nullopt;
    }
    return parentIt->second;
}

const EntityHierarchy::Children& EntityHierarchy::getChildren(Entity parent) const
{
    const auto& childrenIt = _parentToChildren.find(parent);
    if(childrenIt == _parentToChildren.end())
    {
        static const Children emptyChildren = {};
        return emptyChildren;
    }
    return childrenIt->second;
}

bool EntityHierarchy::isAncestor(Entity entity, Entity ancestor) const
{
    if(entity == ancestor)
    {
        return true;
    }
    if(!_childToParent.contains(entity))
    {
        return false;
    }
    return isAncestor(_childToParent.at(entity), ancestor);
}

}