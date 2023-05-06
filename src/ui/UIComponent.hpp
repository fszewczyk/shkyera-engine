#pragma once

#include <unordered_map>

#include "game/GameObject.hpp"

namespace shkyera {

class UIComponent {
  public:
    UIComponent(std::string name, std::shared_ptr<GameObject> object);

    virtual void draw() = 0;

    std::string _name;
    std::shared_ptr<GameObject> _object;

    static void addComponentToObject(std::shared_ptr<GameObject> object, std::shared_ptr<UIComponent> component);
    static std::vector<std::shared_ptr<UIComponent>> getComponentsOfObject(std::shared_ptr<GameObject> object);

  protected:
    uint64_t _uuid;

  private:
    static std::unordered_map<std::shared_ptr<GameObject>, std::vector<std::shared_ptr<UIComponent>>> _objectComponents;
};

} // namespace shkyera