#pragma once

#include <Common/TypeInfo.hpp>
#include <Common/Types.hpp>
#include <ECS/EntityHierarchy.hpp>
#include <ECS/Registry.hpp>
#include <JobSystem/Requirements.hpp>
#include <optional>
#include <utility>

namespace shkyera {

template <typename... Resources>
struct ReadAccess {
  ReadAccess() {
    (resources.emplace(TypeInfo<Resources>::ID), ...);
    onlyMainThread = (false || ... || OnlyMainThread<Resources>);
  }
  TypeSet resources;
  bool onlyMainThread;
};

template <typename... Resources>
struct WriteAccess {
  WriteAccess() {
    (resources.emplace(TypeInfo<Resources>::ID), ...);
    onlyMainThread = (false || ... || OnlyMainThread<Resources>);
  }
  TypeSet resources;
  bool onlyMainThread;
};

struct Policy {
  TypeSet read;
  TypeSet write;
  bool onlyMainThread;

  Policy(TypeSet readSet, TypeSet writeSet, bool mainThread)
      : read(std::move(readSet)), write(std::move(writeSet)), onlyMainThread(mainThread) {}

  template <typename Type>
  bool canRead() const {
    return read.contains(TypeInfo<Type>::ID) || write.contains(TypeInfo<Type>::ID);
  }
  template <typename Type>
  bool canWrite() const {
    return write.contains(TypeInfo<Type>::ID);
  }
};

class RegistryViewer {
 public:
  template <typename... Resources>
  RegistryViewer(std::shared_ptr<Registry> registry, ReadAccess<Resources...> read)
      : _registry(std::move(registry)), _policy(std::move(read.resources), {}, read.onlyMainThread) {}

  template <typename... ReadResources, typename... WriteResources>
  RegistryViewer(std::shared_ptr<Registry> registry, ReadAccess<ReadResources...> read,
                 WriteAccess<WriteResources...> write)
      : _registry(std::move(registry)),
        _policy(std::move(read.resources), std::move(write.resources), read.onlyMainThread || write.onlyMainThread) {}

  const Policy& getPolicy() const { return _policy; }

  template <typename Component, typename... Args>
  Component& add(Entity entity, Args&&... args) {
    SHKYERA_ASSERT(_policy.canWrite<Component>(), "Cannot write {}. Policy does not allow it",
                   typeid(Component).name());

    return _registry->addComponent<Component>(entity, std::forward<Args>(args)...);
  }

  template <typename Component>
  void clear() {
    SHKYERA_ASSERT(_policy.canWrite<Component>(), "Cannot write {}. Policy does not allow it",
                   typeid(Component).name());

    return _registry->clearComponents<Component>();
  }

  template <typename Component>
  bool has(Entity entity) const {
    SHKYERA_ASSERT(_policy.canRead<Component>(), "Cannot write {}. Policy does not allow it", typeid(Component).name());

    return _registry->hasComponent<Component>(entity);
  }

  template <typename Component>
  Component& write(Entity entity) {
    SHKYERA_ASSERT(_policy.canWrite<Component>(), "Cannot write {}. Policy does not allow it",
                   typeid(Component).name());

    return _registry->getComponent<Component>(entity);
  }

  template <typename Component>
  const Component& read(Entity entity) const {
    SHKYERA_ASSERT(_policy.canRead<Component>(), "Cannot read {}. Policy does not allow it", typeid(Component).name());

    return _registry->getComponent<Component>(entity);
  }

  template <NonSingletonComponentType Component>
  const auto& read() const {
    SHKYERA_ASSERT(_policy.canRead<Component>(), "Cannot read {}. Policy does not allow it", typeid(Component).name());

    return _registry->getComponents<Component>();
  }

  template <SingletonComponentType Component>
  const auto* read() const {
    SHKYERA_ASSERT(_policy.canRead<Component>(), "Cannot read {}. Policy does not allow it", typeid(Component).name());

    return _registry->getComponent<Component>();
  }

  template <SingletonComponentType Component>
  auto* write() {
    SHKYERA_ASSERT(_policy.canRead<Component>(), "Cannot write {}. Policy does not allow it", typeid(Component).name());

    return _registry->getComponent<Component>();
  }

  template <SingletonComponentType Component>
  std::optional<Entity> getEntity() const {
    return _registry->getEntity<Component>();
  }

  const std::optional<Entity> getParent(Entity entity) const {
    SHKYERA_ASSERT(_policy.canRead<EntityHierarchy>(), "Cannot get parent of {}. Policy does not allow it",
                   typeid(EntityHierarchy).name());

    return _registry->getHierarchy().getParent(entity);
  }

  template <typename Component>
  SparseSet<Component>& writeAll() {
    SHKYERA_ASSERT(_policy.canWrite<Component>(), "Cannot write {}. Policy does not allow it",
                   typeid(Component).name());

    return _registry->getComponentSet<Component>();
  }

  template <typename Component>
  const SparseSet<Component>& readAll() const {
    SHKYERA_ASSERT(_policy.canRead<Component>(), "Cannot write {}. Policy does not allow it", typeid(Component).name());

    return _registry->getComponentSet<Component>();
  }

 private:
  std::shared_ptr<Registry> _registry;
  Policy _policy;
};

}  // namespace shkyera
