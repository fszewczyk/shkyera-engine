#pragma once

#include <ECS/Registry.hpp>
#include <memory>

namespace shkyera {

class ObjectSelectionSystem {
 public:
  ObjectSelectionSystem(std::shared_ptr<Registry> registry);

 private:
  std::optional<Entity> getHoveredObject();

  std::optional<Entity> _objectSelectedOnMouseDown;
  std::shared_ptr<Registry> _registry;
};

}  // namespace shkyera
