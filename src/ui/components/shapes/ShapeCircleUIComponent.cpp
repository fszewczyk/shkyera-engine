#include "imgui.h"
#include <iostream>

#include "core/Image.hpp"
#include "game/components/shapes/ShapeCircleComponent.hpp"
#include "ui/components/shapes/ShapeCircleUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ShapeCircleUIComponent::drawOptions() {
    static float r = 10;
    ImGui::SliderFloat("Radius", &r, 0.0, 500.0);

    _shape->setRadius(r);
}
void ShapeCircleUIComponent::initialize(std::shared_ptr<Renderer> renderer) {
    _shape = std::make_shared<ShapeCircleComponent>(_object);
    _shape->setRenderer(renderer);

    ShapeComponent::addShape(_shape);
}

} // namespace shkyera
