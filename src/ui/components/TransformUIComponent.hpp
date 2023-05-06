#pragma once

#include "ui/Component.hpp"

namespace shkyera {

class TransformUIComponent : public UIComponent {
    using UIComponent::UIComponent;

    virtual void draw() override;
};

} // namespace shkyera
