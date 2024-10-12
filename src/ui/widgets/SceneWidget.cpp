#include <imgui_internal.h>
#include "imgui.h"

#include <iostream>

#include <AssetManager/Image.hpp>
#include <UI/Widgets/ConsoleWidget.hpp>
#include <UI/Widgets/SceneWidget.hpp>

namespace shkyera {

SceneWidget::SceneWidget(std::shared_ptr<Registry> registry) : Widget("Scene"), _runtime(registry) {
  _runtime.run();
}


void SceneWidget::draw() {
  ImGui::Begin(
      _name.c_str(), nullptr,
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

  ImGui::Dummy(ImVec2(ImGui::GetWindowSize()[0] / 2 - 48, 16));
  ImGui::SameLine();

  drawScene();

  ImGui::End();
}

void SceneWidget::adjustSize() {
  _renderSize = ImGui::GetWindowSize();
  _renderSize[0] -= 16;
  _renderSize[1] -= 16;
}

void SceneWidget::drawScene() {}

}  // namespace shkyera
