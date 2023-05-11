#include "imgui.h"
#include <imgui_internal.h>

#include "python/Interpreter.hpp"
#include "ui/widgets/SceneWidget.hpp"

namespace shkyera {

void SceneWidget::draw() {
    ImGui::Begin(_name.c_str());
    if (ImGui::Button("Python")) {
        python::run();
    }
    ImGui::End();
}

} // namespace shkyera