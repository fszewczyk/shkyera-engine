#pragma once

#include "ui/Widget.hpp"

namespace shkyera {

class PropertiesWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() override;
};

} // namespace shkyera