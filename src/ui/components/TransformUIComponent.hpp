#pragma once

#include "ui/UIComponent.hpp"

namespace shkyera {

class TransformUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    virtual void draw() override;
};

} // namespace shkyera
