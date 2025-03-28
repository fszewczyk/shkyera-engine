#include <UI/Common/Utils.hpp>
#include <UI/Common/Style.hpp>

#include <imgui_internal.h>
#include "imgui.h"

namespace shkyera::utils::ui {

void drawCenteredTitle(std::string_view text, ImFont* font)
{
  auto windowWidth = ImGui::GetWindowSize().x;
  auto textWidth   = ImGui::CalcTextSize(text.data()).x;

  ImGui::Spacing();
  ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
  ImGui::PushFont(font);
  ImGui::TextUnformatted(text.data());
  ImGui::PopFont();
}

}