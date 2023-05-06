#pragma once

#include "game/GameObject.hpp"
#include "ui/UIComponent.hpp"
#include "ui/Widget.hpp"

namespace shkyera {

class PropertiesWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() override;

    static void setObject(std::shared_ptr<GameObject> object);

  private:
    void drawNewComponentMenu();

    static std::shared_ptr<GameObject> _object;
    static std::vector<std::shared_ptr<UIComponent>> _components;
};

} // namespace shkyera