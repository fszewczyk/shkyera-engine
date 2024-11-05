#include <UI/Common/Style.hpp>

namespace shkyera::style {

void loadFonts(const std::string& path) {
  SMALL_FONT = ImGui::GetIO().Fonts->AddFontFromFileTTF(
      path.c_str(), 14);  ///< A smaller font used in the UI.
  NORMAL_FONT = ImGui::GetIO().Fonts->AddFontFromFileTTF(
      path.c_str(), 16);  ///< The normal font used in the UI.
  BIG_FONT = ImGui::GetIO().Fonts->AddFontFromFileTTF(
      path.c_str(), 20);  ///< A larger font used in the UI.
  HUGE_FONT = ImGui::GetIO().Fonts->AddFontFromFileTTF(
      path.c_str(), 24);  ///< An even larger font used in the UI.
}

/********        FONTS        ********/
ImFont* SMALL_FONT = nullptr;
ImFont* NORMAL_FONT = nullptr;
ImFont* BIG_FONT = nullptr;
ImFont* HUGE_FONT = nullptr;

/********        COLORS      *********/
const ImVec4 BACKGROUND_COLOR = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
const ImVec4 TEXT_COLOR = ImVec4(0.86f, 0.86f, 0.86f, 1.0f);
const ImVec4 DISABLED_TEXT_COLOR = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
const ImVec4 ACCENT_COLOR = ImVec4(0.22f, 0.14f, 0.7f, 1.0f);
const ImVec4 STRONG_ACCENT_COLOR =  ImVec4(0.46f, 0.02f, 0.9f, 1.0f);
const ImVec4 GREY = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
const ImVec4 LIGHT_GREY = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
const ImVec4 DARK_ACCENT = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
const ImVec4 BLACK = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

}  // namespace shkyera::style