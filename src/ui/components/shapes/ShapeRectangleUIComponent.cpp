#include "imgui.h"

#include "core/Image.hpp"
#include "ui/components/shapes/ShapeRectangleUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ShapeRectangleUIComponent::drawOptions() { ImGui::Text("Rectangle"); }

} // namespace shkyera
