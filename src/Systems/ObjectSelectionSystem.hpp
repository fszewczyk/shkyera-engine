#pragma once

#include <ECS/Registry.hpp>
#include <ECS/RegistryViewer.hpp>

#include <memory>

namespace shkyera {

class ObjectSelectionSystem : public RegistryViewer {
 public:
  ObjectSelectionSystem(std::shared_ptr<Registry> registry);

  void update();

 private:
  std::optional<Entity> getHoveredObject();

  std::optional<Entity> _objectSelectedOnMouseDown;
};

}  // namespace shkyera
