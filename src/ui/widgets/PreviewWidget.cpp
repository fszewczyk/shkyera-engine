#include <imgui_internal.h>
#include "imgui.h"

#include <UI/Widgets/PreviewWidget.hpp>

namespace shkyera {

void PreviewWidget::draw() {
  ImGui::Begin(_name.c_str());
  ImGui::End();
}

}  // namespace shkyera