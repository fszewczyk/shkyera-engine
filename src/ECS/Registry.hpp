/**
 * @file Registry.hpp
 * 
 * Contains the collection of entities.
 */

#pragma once

#include <unordered_set>

#include <Common/Logger.hpp>
#include <ECS/Entity.hpp>
#include <ECS/EntityHierarchy.hpp>
#include <ECS/EntityProvider.hpp>
#include <ECS/SingletonComponent.hpp>
#include <ECS/SparseSet.hpp>
#include "cereal/cereal.hpp"

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

    const std::unordered_set<Entity>& getSelectedEntities();

    void selectEntity(Entity entity);

    void clearSelectedEntities();

    /**
     * Adds a component to the specified entity.
     * Initializes the component with default values.
     * 
     * @tparam Component Type of the component to add.
     * @param entity The entity to which the component is added.
     * @return Refence to the constructed component
     */
    template <typename Component, typename... Args>
    Component& addComponent(Entity entity, Args&&... args) {
        auto& componentSet = getOrCreateComponentSet<Component>();
        if constexpr (std::is_base_of_v<SingletonComponent, Component>) {
            if (componentSet.contains(entity)) {
                Logger::ERROR(std::string("Cannot add a Singleton Component (") + typeid(Component).name() + "), because another entity already has it.");
                return componentSet.get(entity);
            }
        }
        componentSet.add(entity, Component(std::forward<Args>(args)...));
        return componentSet.get(entity);
    }

    template <typename Component, typename... Args>
    Component& assignComponent(Entity entity, Args&&... args) {
        static_assert(std::is_base_of_v<SingletonComponent, Component>, "Component assignment is only possible for Singleton Components.");

        auto& componentSet = getOrCreateComponentSet<Component>();
        componentSet.clear();
        return addComponent<Component>(entity, std::forward<Args>(args)...);
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
     * Checks if the specified entity has all of the specified components.
     * 
     * @tparam Components Types of the components to check for.
     * @param entity The entity to check.
     * @return True if the entity has all the components, false otherwise.
     */
    template <typename... Components>
    bool hasComponents(Entity entity) const {
        return (... && (hasComponent<Components>(entity)));
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

    Entity getEnvironment() const;

    EntityHierarchy& getHierarchy();

    const EntityHierarchy& getHierarchy() const;

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(_entityProvider));
        archive(CEREAL_NVP(_entityHierarchy));
        archive(CEREAL_NVP(_selectedEntities));
        archive(CEREAL_NVP(_camera));
        archive(CEREAL_NVP(_environment));
    }

   private:
    using ParentAndChild = std::pair<Entity, Entity>;

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

    mutable std::unordered_map<size_t, std::unique_ptr<SparseSetBase>> _componentSets;  //< Map of component sets by type ID.
    EntityProvider _entityProvider;                                                     //< Manages the creation and management of entities.
    EntityHierarchy _entityHierarchy;                                                   //< Maintains the parent-child relationships between the entities

    Entity _camera;
    Entity _environment;
    std::unordered_set<Entity> _selectedEntities;
};

}  // namespace shkyera
