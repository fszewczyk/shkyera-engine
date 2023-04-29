#pragma once

#include <memory>
#include <vector>

#include "core/Entity.hpp"

namespace shkyera {

class GameObject : public Entity {
  public:
    GameObject(std::string name);

    std::string getName() const;

  private:
    std::string _name;
};

} // namespace shkyera