#include <Systems/PhysicsSystem.hpp>

namespace shkyera {

PhysicsSystem::PhysicsSystem(std::shared_ptr<Registry> registry) : _registry(std::move(registry)) {}

void PhysicsSystem::update() {}

}  // namespace shkyera
