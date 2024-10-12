#pragma once

#include <ECS/EntityProvider.hpp>

namespace shkyera {

/**
 * Manages a sparse set of components associated with entities.
 * Allows adding, removing, and accessing components efficiently.
 */
template <typename Component>
class SparseSet {
public:
    /**
     * Default constructor.
     */
    SparseSet() = default;

    /**
     * Default destructor.
     */
    ~SparseSet() = default;

    /**
     * Adds a component for the specified entity.
     * 
     * @param entity The entity to associate with the component.
     * @param component The component to add.
     */
    void add(Entity entity, const Component &component) {
        _entityToComponent[entity] = _entities.size();
        _entities.emplace_back(entity);
        _components.emplace_back(component);
    }

    /**
     * Removes the component associated with the specified entity.
     * 
     * @param entity The entity whose component is to be removed.
     * @return True if the component was successfully removed, false otherwise.
     */
    bool remove(Entity entity) {
        if (!contains(entity)) {
            return false;
        }

        const auto& index = _entityToComponent.at(entity);
        const auto& lastIndex = _entities.size() - 1;

        _entities[index] = _entities[lastIndex];
        _components[index] = _components[lastIndex];

        _entityToComponent[_entities.at(index)] = index;

        _entities.pop_back();
        _components.pop_back();
        _entityToComponent.erase(entity);

        return true;
    }

    /**
     * Checks if the specified entity has an associated component.
     * 
     * @param entity The entity to check.
     * @return True if the entity has an associated component, false otherwise.
     */
    bool contains(Entity entity) const {
        return _entityToComponent.find(entity) != _entityToComponent.end();
    }

    /**
     * Retrieves a reference to the component associated with the specified entity.
     * 
     * @param entity The entity whose component is to be retrieved.
     * @return Reference to the associated component.
     */
    Component& get(Entity entity) {
        return _components[_entityToComponent.at(entity)];
    }

    /**
     * Retrieves a const reference to the component associated with the specified entity.
     * 
     * @param entity The entity whose component is to be retrieved.
     * @return Const reference to the associated component.
     */
    const Component &get(Entity entity) const {
        return _components[_entityToComponent.at(entity)];
    }

    /**
     * Retrieves a vector of all components in the sparse set.
     * 
     * @return Reference to the vector of components.
     */
    std::vector<Component> &getComponents() {
        return _components;
    }

    /**
     * Retrieves a const vector of all components in the sparse set.
     * 
     * @return Const reference to the vector of components.
     */
    const std::vector<Component> &getComponents() const {
        return _components;
    }

private:
    std::vector<Entity> _entities; /**< Stores entities associated with components. */
    std::unordered_map<Entity, size_t> _entityToComponent; /**< Maps entities to their component indices. */
    std::vector<Component> _components; /**< Stores components associated with entities. */
};

} // namespace shkyera
