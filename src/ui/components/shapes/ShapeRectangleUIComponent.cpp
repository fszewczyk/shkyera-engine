#include "imgui.h"

#include "core/Image.hpp"
#include "ui/components/shapes/ShapeRectangleUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ShapeRectangleUIComponent::drawOptions() {
    static float width = 10, height = 10;
    static float color[4] = {0, 0, 0, 255};

    ImGui::SliderFloat("Width", &width, 0.0f, 1000.0f);
    ImGui::SliderFloat("Height", &height, 0.0f, 1000.0f);
    ImGui::ColorPicker3("Color", color);

    _shape->setWidth(width);
    _shape->setHeight(height);
    _shape->setColor({255 * color[0], 255 * color[1], 255 * color[2]});
}

void ShapeRectangleUIComponent::initialize(std::shared_ptr<Renderer> renderer) {
    _shape = std::make_shared<ShapeRectangleComponent>(_object);
    _shape->setRenderer(renderer);

    ShapeComponent::addShape(_shape);
}

} // namespace shkyera
