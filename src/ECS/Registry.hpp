/**
 * @file Registry.hpp
 * 
 * Contains the collection of entities.
 */

#pragma once

#include <ECS/EntityProvider.hpp>
#include <ECS/SparseSet.hpp>

namespace shkyera {

/**
 * Manages a collection of entities and their components.
 * Provides methods to add, remove, and query entities and components.
 */
class Registry {
public:
    /**
     * Default constructor. Initializes the registry for managing entities and components.
     */
    Registry();

    /**
     * Default destructor.
     */
    ~Registry() = default;

    /**
     * Adds a new entity to the registry.
     * 
     * @return The newly created entity.
     */
    Entity addEntity();

    /**
     * Removes an entity from the registry.
     * 
     * @param entity The entity to remove.
     */
    void removeEntity(Entity entity);

    std::vector<Entity> getSelectedEntities();

    void selectEntity(Entity entity);

    void clearSelectedEntities();

    /**
     * Adds a component to the specified entity.
     * Initializes the component with default values.
     * 
     * @tparam Component Type of the component to add.
     * @param entity The entity to which the component is added.
     */
    template <typename Component>
    void addComponent(Entity entity) {
        getOrCreateComponentSet<Component>().add(entity, Component());
    }

    /**
     * Removes a component from the specified entity.
     * 
     * @tparam Component Type of the component to remove.
     * @param entity The entity from which the component is removed.
     */
    template <typename Component>
    void removeComponent(Entity entity) {
        getOrCreateComponentSet<Component>().remove(entity);
    }

    /**
     * Checks if the specified entity has a specific component.
     * 
     * @tparam Component Type of the component to check for.
     * @param entity The entity to check.
     * @return True if the entity has the component, false otherwise.
     */
    template <typename Component>
    bool hasComponent(Entity entity) const {
        return getOrCreateComponentSet<Component>().contains(entity);
    }

    /**
     * Retrieves a reference to the component of the specified entity.
     * 
     * @tparam Component Type of the component to retrieve.
     * @param entity The entity whose component is retrieved.
     * @return Reference to the component.
     */
    template <typename Component>
    Component& getComponent(Entity entity) {
        return getOrCreateComponentSet<Component>().get(entity);
    }

    /**
     * Retrieves a const reference to the component of the specified entity.
     * 
     * @tparam Component Type of the component to retrieve.
     * @param entity The entity whose component is retrieved.
     * @return Const reference to the component.
     */
    template <typename Component>
    const Component& getComponent(Entity entity) const {
        return getOrCreateComponentSet<Component>().get(entity);
    }

    /**
     * Retrieves the component set for the specified component type.
     * 
     * @tparam Component Type of the component to retrieve the set for.
     * @return Reference to the component set.
     */
    template <typename Component>
    SparseSet<Component>& getComponentSet() {
        return getOrCreateComponentSet<Component>();
    }

    /**
     * Retrieves the component set for the specified component type.
     * 
     * @tparam Component Type of the component to retrieve the set for.
     * @return Reference to the component set.
     */
    template <typename Component>
    const SparseSet<Component>& getComponentSet() const {
        return getOrCreateComponentSet<Component>();
    }

    template <typename Component>
    std::vector<Component>& getComponents() {
        return getOrCreateComponentSet<Component>().getComponents();
    }

    template <typename Component>
    const std::vector<Component>& getComponents() const {
        return getOrCreateComponentSet<Component>().getComponents();
    }

    Entity getCamera() const;

private:
    /**
     * Retrieves or creates the component set for the specified component type.
     * 
     * @tparam Component Type of the component to get or create the set for.
     * @return Reference to the component set.
     */
    template <typename Component>
    SparseSet<Component>& getOrCreateComponentSet() {
        const auto typeId = typeid(Component).hash_code();
        if (_componentSets.find(typeId) == _componentSets.end()) {
            _componentSets[typeId] = std::make_unique<SparseSet<Component>>();
        }
        return *static_cast<SparseSet<Component>*>(_componentSets.at(typeId).get());
    }

    /**
     * Retrieves or creates the component set for the specified component type.
     * 
     * @tparam Component Type of the component to get or create the set for.
     * @return Reference to the component set.
     */
    template <typename Component>
    const SparseSet<Component>& getOrCreateComponentSet() const {
        const auto typeId = typeid(Component).hash_code();
        if (_componentSets.find(typeId) == _componentSets.end()) {
            _componentSets[typeId] = std::make_unique<SparseSet<Component>>();
        }
        return *static_cast<SparseSet<Component>*>(_componentSets.at(typeId).get());
    }

    mutable std::unordered_map<size_t, std::unique_ptr<SparseSetBase>> _componentSets; //< Map of component sets by type ID.
    EntityProvider _entityProvider; //< Manages the creation and management of entities.

    Entity _camera;
    std::vector<Entity> _selectedEntities;
};

} // namespace shkyera
