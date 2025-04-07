/**
 * @file EntityProvider.hpp
 * @brief Manages a pool of unused entity identifiers and provides mechanisms for requesting and removing them.
 */

#pragma once

#include <stdlib.h>
#include <queue>

#include <ECS/Entity.hpp>

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
         */
  EntityProvider();

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

 private:
  /**
         * Tracks the total number of entity IDs allocated so far.
         */
  Entity _nextEntity;
};

}  // namespace shkyera
