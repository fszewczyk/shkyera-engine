#include "imgui.h"

#include "core/Image.hpp"
#include "ui/components/TransformUIComponent.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

namespace shkyera {

void TransformUIComponent::draw() {
    ImGui::Image((ImTextureID)Image::ICON_COMPONENT_TRANSFORM.getTextureId(), ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNodeEx(_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        glm::vec3 &position = _object->getPosition();
        glm::vec3 &orientation = _object->getOrientation();
        glm::vec3 &scale = _object->getScale();

        ImGui::PushItemWidth(50);

        ImGui::Text("Position");
        ImGui::SameLine(120);
        ImGui::DragFloat("X##Position", &position[0], 1.0f, 0.0f, 0.0f);
        ImGui::SameLine();
        ImGui::DragFloat("Y##Position", &position[1], 1.0f, 0.0f, 0.0f);
        ImGui::SameLine();
        ImGui::DragFloat("Z##Position", &position[2], 1.0f, 0.0f, 0.0f);

        ImGui::Text("Orientation");
        ImGui::SameLine(120);
        ImGui::DragFloat("X##Orientation", &orientation[0], 0.1f, -180.0f, 180.0f);
        ImGui::SameLine();
        ImGui::DragFloat("Y##Orientation", &orientation[1], 0.1f, -180.0f, 180.0f);
        ImGui::SameLine();
        ImGui::DragFloat("Z##Orientation", &orientation[2], 0.1f, -180.0f, 180.0f);

        ImGui::Text("Scale");
        ImGui::SameLine(120);
        ImGui::DragFloat("X##Scale", &scale[0], 0.01f, 0.0f, 1000.0f);
        ImGui::SameLine();
        ImGui::DragFloat("Y##Scale", &scale[1], 0.01f, 0.0f, 1000.0f);
        ImGui::SameLine();
        ImGui::DragFloat("Z##Scale", &scale[2], 0.01f, 0.0f, 1000.0f);

        ImGui::PopItemWidth();

        ImGui::TreePop();
    }
}

} // namespace shkyera
