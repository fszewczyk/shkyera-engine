#include "imgui.h"

#include <UI/UI.hpp>
#include <UI/widgets/PropertiesWidget.hpp>

namespace shkyera {

void PropertiesWidget::draw() {
    ImGui::Begin(_name.c_str());

    ImGui::Text("No object has been selected.");

    ImGui::End();
}

} // namespace shkyera
