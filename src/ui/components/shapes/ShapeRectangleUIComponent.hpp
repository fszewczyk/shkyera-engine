#pragma once

#include "ui/components/shapes/ShapeUIComponent.hpp"

namespace shkyera {

class ShapeRectangleUIComponent : public ShapeUIComponent {
    using ShapeUIComponent::ShapeUIComponent;

    virtual void drawOptions() override;
};

} // namespace shkyera
