#pragma once

#include "ui/components/shapes/ShapeUIComponent.hpp"

namespace shkyera {

class ShapeLineUIComponent : public ShapeUIComponent {
    using ShapeUIComponent::ShapeUIComponent;

    virtual void drawOptions() override;
};

} // namespace shkyera
