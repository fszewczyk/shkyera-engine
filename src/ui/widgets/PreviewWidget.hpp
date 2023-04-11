#pragma once

#include "ui/Widget.hpp"

namespace shkyera {

class PreviewWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() const override;
};

} // namespace shkyera