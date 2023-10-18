#pragma once

#include "ui/UIComponent.hpp"

namespace shkyera {

class ShapeUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    virtual void draw() override;
    virtual void drawOptions() = 0;
};

} // namespace shkyera
