#include "imgui.h"
#include <imgui_internal.h>

#include "ui/widgets/PreviewWidget.hpp"

namespace shkyera {

void PreviewWidget::draw() {
    ImGui::Begin(_name.c_str());
    ImGui::End();
}

} // namespace shkyera