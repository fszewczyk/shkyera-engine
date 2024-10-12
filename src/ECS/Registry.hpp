/**
 * @file Registry.hpp
 *
 * @brief Contains the collection of entities.
 */

#pragma once

#include <ECS/EntityProvider.hpp>
#include <ECS/SparseSet.hpp>

namespace shkyera {

class Registry
{
    public:
        Registry();

        ~Registry() = default;

        Entity addEntity();

        void removeEntity(Entity entity);

        template <typename Component>
        void addComponent(Entity entity) {
            getOrCreateComponentSet<Component>().add(entity, Component());
        }

        template <typename Component>
        void removeComponent(Entity entity) {
            getOrCreateComponentSet<Component>().remove(entity);
        }

        template <typename Component>
        bool hasComponent(Entity entity) const {
            return getOrCreateComponentSet<Component>().contains(entity);
        }

        template <typename Component>
        Component& getComponent(Entity entity) {
            return getOrCreateComponentSet<Component>().get(entity);
        }

        template <typename Component>
        const Component& getComponent(Entity entity) const {
            return getOrCreateComponentSet<Component>().get(entity);
        }

        template <typename Component>
        SparseSet<Component>& getComponentSet() {
            return getOrCreateComponentSet<Component>();
        }

    private:
        template <typename Component>
        SparseSet<Component>& getOrCreateComponentSet() {
            const auto typeId = typeid(Component).hash_code();
            if (_componentSets.find(typeId) == _componentSets.end()) {
                _componentSets[typeId] = new SparseSet<Component>();
            }
            return *static_cast<SparseSet<Component>*>(_componentSets.at(typeId));
        }

        std::unordered_map<size_t, void*> _componentSets;
        EntityProvider _entityProvider;
};

}
