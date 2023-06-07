#include "imgui.h"
#include <imgui_internal.h>

#include <iostream>

#include "game/components/ScriptComponent.hpp"
#include "python/Interpreter.hpp"
#include "ui/widgets/ConsoleWidget.hpp"
#include "ui/widgets/SceneWidget.hpp"

namespace shkyera {

void SceneWidget::draw() {
    ImGui::Begin(_name.c_str());
    if (ImGui::Button("Python")) {
        ScriptComponent::moveScripts();
        Python::start();
    }
    ImGui::End();
}

} // namespace shkyera
