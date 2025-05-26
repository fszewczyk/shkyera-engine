#pragma once

#include <ECS/Registry.hpp>

namespace shkyera {

class PhysicsSystem {
 public:
  explicit PhysicsSystem(std::shared_ptr<Registry> registry);

  void update();

 private:
  std::shared_ptr<Registry> _registry;
};

}  // namespace shkyera
