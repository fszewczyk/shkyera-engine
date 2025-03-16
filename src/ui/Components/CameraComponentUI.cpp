#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/Components/CameraComponentUI.hpp>
#include <glm/glm.hpp>

namespace shkyera {

CameraComponentUI::CameraComponentUI(CameraComponent* cameraComponent)
    : _cameraComponent(cameraComponent),
      _fovSlider("Field of View", cameraComponent->fov, 5, 160),
      _projectionSelector("Projection", cameraComponent->projectionType, {
                                                                             {CameraComponent::ProjectionType::Perspective, "Perspective"},
                                                                             {CameraComponent::ProjectionType::Orthographic, "Orthographic"},
                                                                         }) {
    _fovSlider.setUpdateCallback([this](float fov) {
        _cameraComponent->fov = fov;
    });
    _projectionSelector.setUpdateCallback([this](const auto projection) {
        _cameraComponent->projectionType = projection;
    });
}

void CameraComponentUI::draw() {
    ImGui::Image(_icon->getImguiTextureID(),
                 ImVec2(16, 16));
    ImGui::SameLine();
    if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        _projectionSelector.draw();
        _fovSlider.draw();

        ImGui::TreePop();
    }
}

}  // namespace shkyera
