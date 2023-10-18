#include "imgui.h"

#include "core/Image.hpp"
#include "game/components/shapes/ShapeLineComponent.hpp"
#include "ui/components/shapes/ShapeLineUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ShapeLineUIComponent::drawOptions() {
    static float x = 10, y = 10;

    ImGui::SliderFloat("X", &x, -1000.0f, 1000.0f);
    ImGui::SliderFloat("Y", &y, -1000.0f, 1000.0f);

    _shape->setX(x);
    _shape->setY(y);
}
void ShapeLineUIComponent::initialize(std::shared_ptr<Renderer> renderer) {
    _shape = std::make_shared<ShapeLineComponent>(_object);
    _shape->setRenderer(renderer);

    ShapeComponent::addShape(_shape);
}

} // namespace shkyera
