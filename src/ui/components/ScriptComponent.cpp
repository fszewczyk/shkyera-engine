#include "imgui.h"

#include "core/Image.hpp"
#include "ui/components/ScriptComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void ScriptComponent::draw() {
    ImGui::Image((ImTextureID)Image::ICON_COMPONENT_SCRIPT.getTextureId(), ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNode(_name.c_str())) {
        ImGui::Text("Python Script Data");
        ImGui::TreePop();
    }
}

} // namespace shkyera