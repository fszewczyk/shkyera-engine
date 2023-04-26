#pragma once

#include <memory>
#include <vector>

#include "components/Component.hpp"
#include "core/Entity.hpp"

namespace shkyera {

class GameObject : public Entity {
  public:
    GameObject(std::string name);
    void addComponent(std::shared_ptr<Component> component);

    std::string getName() const;

  private:
    std::string _name;
    std::vector<std::shared_ptr<Component>> _components;
};

} // namespace shkyera