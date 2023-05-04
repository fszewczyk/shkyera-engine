#pragma once

#include "ui/Component.hpp"

namespace shkyera {

class ScriptComponent : public UIComponent {
    using UIComponent::UIComponent;

    virtual void draw() override;
};

} // namespace shkyera