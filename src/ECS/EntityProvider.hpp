/**
 * @file EntityProvider.hpp
 * @brief Manages a pool of unused entity identifiers and provides mechanisms for requesting and removing them.
 */

#pragma once

#include <stdlib.h>
#include <queue>

namespace shkyera {

using Entity = uint32_t;

/**
 * @class EntityProvider
 * @brief Manages a pool of reusable entity identifiers.
 *
 * Tracks available entity IDs and supports requesting new IDs and returning unused ones for reuse.
 */
class EntityProvider
{
    public:
        /**
         * @brief Construct an EntityProvider with a specified number of entities.
         * 
         * @param numEntities The number of entity IDs to initialize.
         *
         * Initializes the pool of entity IDs (from 0 to `numEntities - 1`) and stores them in the queue for reuse.
         */
        explicit EntityProvider(Entity numEntities);

        /**
         * @brief Default destructor.
         *
         * Releases resources used by the internal queue.
         */
        ~EntityProvider() = default;

        /**
         * @brief Request an available entity ID.
         *
         * @return An unused entity ID.
         *
         * Returns an entity ID from the pool of available IDs, allocating more if necessary.
         */
        Entity requestEntity();

        /**
         * @brief Return an entity ID to the pool.
         *
         * @param entity The entity ID to return.
         *
         * Adds the provided entity ID back to the pool, making it available for future requests.
         */
        void removeEntity(Entity entity);

    private:
        /**
         * @brief Allocate additional entity IDs. Guarantees that the new IDs were not used before.
         *
         * @param entitiesToAllocate The number of new entity IDs to add to the pool.
         */
        void allocateEntities(Entity entitiesToAllocate);

        /**
         * @brief The total number of entities managed.
         *
         * Tracks the total number of entity IDs allocated so far.
         */
        Entity _numEntities;

        /**
         * @brief Queue of unused entity IDs.
         *
         * Stores available entity IDs for reuse.
         */
        std::queue<size_t> _entities;
};

}
