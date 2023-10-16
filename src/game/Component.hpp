#pragma once

#include <memory>

#include "game/GameObject.hpp"

namespace shkyera {

class Component {
  public:
    Component(std::shared_ptr<GameObject> object);

    std::shared_ptr<GameObject> getObject();

  protected:
    std::shared_ptr<GameObject> _object;
};

} // namespace shkyera
