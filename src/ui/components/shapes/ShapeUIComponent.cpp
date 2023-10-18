#include "imgui.h"

#include "core/Image.hpp"
#include "ui/components/shapes/ShapeUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ShapeUIComponent::draw() {
    ImGui::Image((ImTextureID)Image::ICON_COMPONENT_SHAPE.getTextureId(), ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNodeEx(_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        drawOptions();
        ImGui::TreePop();
    }
}

} // namespace shkyera
