#include <UI/Components/TransformComponentUI.hpp>

namespace shkyera {

void TransformComponentUI::setPositionGetter(
    std::function<glm::vec3&(void)> getter) {
  _positionGetter = getter;
}
void TransformComponentUI::setOrientationGetter(
    std::function<glm::vec3&(void)> getter) {
  _orientationGetter = getter;
}
void TransformComponentUI::setScaleGetter(
    std::function<glm::vec3&(void)> getter) {
  _scaleGetter = getter;
}

void TransformComponentUI::draw() {
  ImGui::Image((ImTextureID)Image::ICON_COMPONENT_TRANSFORM.getTextureId(),
               ImVec2(16, 16));
  ImGui::SameLine();
  if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
    glm::vec3& position = _positionGetter();
    glm::vec3& orientation = _orientationGetter();
    glm::vec3& scale = _scaleGetter();

    ImGui::PushItemWidth(50);

    ImGui::Text("Position");
    ImGui::SameLine(120);
    ImGui::DragFloat("X##Position", &position[0], 0.1f, 0.0f, 0.0f);
    ImGui::SameLine();
    ImGui::DragFloat("Y##Position", &position[1], 0.1f, 0.0f, 0.0f);
    ImGui::SameLine();
    ImGui::DragFloat("Z##Position", &position[2], 0.1f, 0.0f, 0.0f);

    ImGui::Text("Orientation");
    ImGui::SameLine(120);
    ImGui::DragFloat("X##Orientation", &orientation[0], 1.0f, -360.0f, 360.0f, "%.2f");
    ImGui::SameLine();
    ImGui::DragFloat("Y##Orientation", &orientation[1], 1.0f, -360.0f, 360.0f, "%.2f");
    ImGui::SameLine();
    ImGui::DragFloat("Z##Orientation", &orientation[2], 1.0f, -360.0f, 360.0f, "%.2f");

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

}  // namespace shkyera
