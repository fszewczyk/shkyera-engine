#include <imgui_internal.h>
#include "Utils/AssetUtils.hpp"
#include "imgui.h"

#include <glad/glad.h>

#include <AssetManager/Image.hpp>
#include <AssetManager/Shader.hpp>
#include <Common/Assert.hpp>
#include <Common/Clock.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/CameraTags.hpp>
#include <Components/NameComponent.hpp>
#include <InputManager/InputManager.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/RuntimeWidget.hpp>

namespace shkyera {

RuntimeWidget::RuntimeWidget(std::shared_ptr<Registry> registry)
    : Widget("Runtime"), _registry(registry), _renderingSystem(registry) {
  _playButtonTexture = utils::assets::read<Texture>(Image::ICON_BUTTON_PLAY);
  _stopButtonTexture = utils::assets::read<Texture>(Image::ICON_BUTTON_STOP);
}

void RuntimeWidget::draw() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin(_name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

  auto renderSize = ImGui::GetContentRegionAvail();
  updateWindowCoordinateSystem();

  _renderingSystem.setSize(renderSize.x * 2.0f, renderSize.y * 2.0f);

  {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    ImGui::SetCursorPos(ImVec2(renderSize.x * 0.5f - 8, 24));

    if (clock::Game.isPaused()) {
      if (ImGui::ImageButton((ImTextureID)(intptr_t)_playButtonTexture->getID(), ImVec2(16, 16))) {
        _storedRegistry = std::make_unique<Registry>(*_registry);
        clock::Game.start();
      }
    } else {
      if (ImGui::ImageButton((ImTextureID)(intptr_t)_stopButtonTexture->getID(), ImVec2(16, 16))) {
        clock::Game.pause();
        *_registry = *_storedRegistry;
        _storedRegistry.reset();
      }
    }

    ImGui::PopStyleVar();
  }

  if (_renderingSystem.render()) {
    ImGui::Image((void*)(intptr_t)_renderingSystem.getRenderFrameBuffer(), renderSize);
  }

  ImGui::End();
  ImGui::PopStyleVar();
}

void RuntimeWidget::updateWindowCoordinateSystem() {
  ImVec2 topLeft = ImGui::GetWindowContentRegionMin();
  ImVec2 topRight = ImGui::GetWindowContentRegionMax();

  topLeft.x += ImGui::GetWindowPos().x;
  topLeft.y += ImGui::GetWindowPos().y;
  topRight.x += ImGui::GetWindowPos().x;
  topRight.y += ImGui::GetWindowPos().y;

  const glm::vec2 topLeftVec = {topLeft.x, topLeft.y};
  const glm::vec2 bottomRightVec = {topRight.x, topRight.y};

  InputManager::getInstance().setCoordinateSystem(InputManager::CoordinateSystem::RUNTIME, topLeftVec, bottomRightVec);
}

}  // namespace shkyera
