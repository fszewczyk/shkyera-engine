#pragma once

#include "ui/Widget.hpp"

namespace shkyera {

class SceneWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() override;

  private:
    void drawRuntime() const;
    void drawScene() const;
};

} // namespace shkyera
