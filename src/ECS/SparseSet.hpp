#pragma once

#include <ECS/EntityProvider.hpp>

namespace shkyera {

template <typename Component>
class SparseSet {
    public:
        SparseSet() = default;
        ~SparseSet() = default;

        void add(Entity entity, const Component &component) {
            _entityToComponent[entity] = _entities.size();
            _entities.emplace_back(entity);
            _components.emplace_back(component);
        }

        bool remove(Entity entity) {
            if(!contains(entity))
            {
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

        bool contains(Entity entity) const {
            return _entityToComponent.find(entity) != _entityToComponent.end();
        }

        Component& get(Entity entity) {
            return _components[_entityToComponent.at(entity)];
        }

        const Component &get(Entity entity) const {
            return _components[_entityToComponent.at(entity)];
        }

        std::vector<Component> &getComponents() {
            return _components;
        }

        const std::vector<Component> &getComponents() const {
            return _components;
        }

    private:
        std::vector<Entity> _entities;
        std::unordered_map<Entity, size_t> _entityToComponent;
        std::vector<Component> _components;
};

}
