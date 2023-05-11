#include "imgui.h"
#include <imgui_internal.h>

#include "game/components/ScriptComponent.hpp"
#include "python/Interpreter.hpp"
#include "ui/widgets/ConsoleWidget.hpp"
#include "ui/widgets/SceneWidget.hpp"

namespace shkyera {

void SceneWidget::draw() {
    ImGui::Begin(_name.c_str());
    if (ImGui::Button("Python")) {
        try {
            ScriptComponent::moveScripts();
            Python::run();
        } catch (std::string &error) {
            ConsoleWidget::logError(error);
        }
    }
    ImGui::End();
}

} // namespace shkyera