#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>

#include <ECS/Entity.hpp>

namespace shkyera {

/**
 * This class provides functionalities to attribute child entities to parent entities,
 * retrieve parent entities, retrieve children of a parent entity, and manage the removal
 * of entities from their parents. It ensures a well-defined tree structure for entity relationships
 * with NO cyclic relations.
 */
class EntityHierarchy {
   public:
    /// Alias for a set of child entities.
    using Children = std::unordered_set<Entity>;

    /**
         * @brief Constructs a new EntityHierarchy object.
         */
    EntityHierarchy() = default;

    /**
         * @brief Destroys the EntityHierarchy object.
         */
    ~EntityHierarchy() = default;

    /**
         * @brief Attributes a child entity to a parent entity.
         *
         * Establishes a parent-child relationship by associating the specified child with the specified parent.
         *
         * @param parent The parent entity.
         * @param child The child entity to be attributed to the parent.
         */
    void attributeChild(Entity parent, Entity child);

    /**
         * @brief Removes a child entity from its parent.
         *
         * If the specified child entity has a parent, the parent-child association is removed.
         *
         * @param child The child entity to be removed from its parent.
         */
    void removeFromParent(Entity child);

    /**
         * @brief Gets the parent of a specified child entity.
         *
         * Retrieves the parent entity of the specified child, if a parent-child relationship exists.
         *
         * @param child The child entity whose parent is to be retrieved.
         * @return An optional containing the parent entity if it exists, or std::nullopt otherwise.
         */
    std::optional<Entity> getParent(Entity child) const;

    /**
         * @brief Gets the children of a specified parent entity.
         *
         * Retrieves all entities that are attributed as children of the specified parent entity.
         *
         * @param parent The parent entity whose children are to be retrieved.
         * @return A constant reference to a set of child entities.
         */
    const Children& getChildren(Entity parent) const;

   private:
    /**
         * @brief Checks if an entity is an ancestor of another entity.
         *
         * Determines if the specified ancestor entity is an ancestor of the specified entity
         * in the hierarchy.
         *
         * @param entity The entity to check.
         * @param ancestor The potential ancestor entity.
         * @return True if the ancestor is an ancestor of the entity, false otherwise.
         */
    bool isAncestor(Entity entity, Entity ancestor) const;

    std::unordered_map<Entity, Children> _parentToChildren;
    std::unordered_map<Entity, Entity> _childToParent;
};

}  // namespace shkyera
