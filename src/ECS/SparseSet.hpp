#pragma once

#include <ECS/EntityProvider.hpp>

namespace shkyera {

class SparseSetBase {
public:
    virtual ~SparseSetBase() = default;
    virtual bool remove(Entity entity) = 0; 
};

/**
 * Manages a sparse set of components associated with entities.
 * Allows adding, removing, and accessing components efficiently.
 */
template <typename Component>
class SparseSet : public SparseSetBase {
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
    bool add(Entity entity, Component component) {
        if(contains(entity)) {
            return false;
        }
        _entityToComponent[entity] = _entities.size();
        _entities.emplace_back(entity);
        _components.push_back(std::move(component));
        
        return true;
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
        _components[index] = std::move(_components[lastIndex]);

        _entityToComponent[_entities.at(index)] = index;

        _entities.pop_back();
        _components.pop_back();
        _entityToComponent.erase(entity);

        return true;
    }

    void clear() {
        _entities.clear();
        _entityToComponent.clear();
        _components.clear();
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

    template <bool IsConst>
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<Entity, std::conditional_t<IsConst, const Component, Component>>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        // Constructor
        Iterator(typename std::conditional_t<IsConst, const std::vector<Entity>*, std::vector<Entity>*> entities,
                 typename std::conditional_t<IsConst, const std::vector<Component>*, std::vector<Component>*> components,
                 size_t index)
            : _entities(entities), _components(components), _index(index) {}

        // Dereference operator
        value_type operator*() const {
            return { (*_entities)[_index], (*_components)[_index] };
        }

        // Pre-increment operator
        Iterator& operator++() {
            ++_index;
            return *this;
        }

        // Post-increment operator
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        // Equality comparison
        bool operator==(const Iterator& other) const {
            return _index == other._index;
        }

        // Inequality comparison
        bool operator!=(const Iterator& other) const {
            return _index != other._index;
        }

    private:
        typename std::conditional_t<IsConst, const std::vector<Entity>*, std::vector<Entity>*> _entities;
        typename std::conditional_t<IsConst, const std::vector<Component>*, std::vector<Component>*> _components;
        size_t _index;
    };

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    iterator begin() {
        return iterator(&_entities, &_components, 0);
    }

    iterator end() {
        return iterator(&_entities, &_components, _entities.size());
    }

    const_iterator begin() const {
        return const_iterator(&_entities, &_components, 0);
    }

    const_iterator end() const {
        return const_iterator(&_entities, &_components, _entities.size());
    }

private:
    std::vector<Entity> _entities; /**< Stores entities associated with components. */
    std::unordered_map<Entity, size_t> _entityToComponent; /**< Maps entities to their component indices. */
    std::vector<Component> _components; /**< Stores components associated with entities. */
};

} // namespace shkyera
