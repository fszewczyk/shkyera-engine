#pragma once

#include "game/GameObject.hpp"

namespace shkyera {

class UIComponent {
  public:
    UIComponent(std::string name, std::shared_ptr<GameObject> object);

    virtual void draw() = 0;

    std::string _name;
    std::shared_ptr<GameObject> _object;
};

} // namespace shkyera