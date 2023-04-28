#include "imgui.h"

#include "ui/components/TransformComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void UIComponent::draw() {
    ImGui::Begin(_name.c_str());

    ImGui::End();
}

} // namespace shkyera