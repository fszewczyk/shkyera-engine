#include "imgui.h"

#include "core/Image.hpp"
#include "ui/components/TransformComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void TransformComponent::draw() {
    ImGui::Image((ImTextureID)Image::ICON_COMPONENT_TRANSFORM.getTextureId(), ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNode(_name.c_str())) {

        ImGui::Text("TODO: Actually implement this. Remember to make sure components are unique (ImGui naming)");

        ImGui::TreePop();
    }
}

} // namespace shkyera