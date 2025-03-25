/**
 * @file EntityProvider.hpp
 * @brief Manages a pool of unused entity identifiers and provides mechanisms for requesting and removing them.
 */

#pragma once

#include <stdlib.h>
#include <Common/Serialization.hpp>
#include <ECS/Entity.hpp>
#include <queue>

namespace shkyera {

/**
 * Manages a pool of reusable entity identifiers.
 * Tracks available entity IDs and supports requesting new IDs and returning unused ones for reuse.
 */
class EntityProvider {
   public:
    /**
         * Construct an EntityProvider with a specified number of entities.
         * Initializes the pool of entity IDs (from 0 to `numEntities - 1`) and stores them in the queue for reuse.
         * 
         * @param numEntities The number of entity IDs to initialize.
         */
    explicit EntityProvider(Entity numEntities);

    /**
         * @brief Default destructor. 
         */
    ~EntityProvider() = default;

    /**
         * @brief Request an available entity ID. Returns an entity ID from the pool of available IDs, allocating more if necessary.
         *
         * @return An unused entity ID.
         */
    Entity requestEntity();

    /**
         * Return an entity ID to the pool. Adds the provided entity ID back to the pool, making it available for future requests.
         *
         * @param entity The entity ID to return.
         */
    void removeEntity(Entity entity);

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(_numEntities);
        archive(_entities);
    }

   private:
    /**
         * Allocate additional entity IDs. Guarantees that the new IDs were not used before.
         *
         * @param entitiesToAllocate The number of new entity IDs to add to the pool.
         */
    void allocateEntities(Entity entitiesToAllocate);

    /**
         * Tracks the total number of entity IDs allocated so far.
         */
    Entity _numEntities;

    /**
         * Stores available entity IDs for use.
         */
    std::queue<size_t> _entities;
};

}  // namespace shkyera
