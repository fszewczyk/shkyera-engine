#pragma once

#include "ui/Widget.hpp"

namespace shkyera {

class PreviewWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() override;

  private:
    std::string _message;
};

} // namespace shkyera
