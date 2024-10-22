#pragma once

#include "imgui.h"

#include <string>

namespace shkyera::style {

void loadFonts(const std::string& path);

/********        FONTS        ********/
extern ImFont *SMALL_FONT;
extern ImFont *NORMAL_FONT;
extern ImFont *BIG_FONT;
extern ImFont *HUGE_FONT;

/********        COLORS      *********/
extern const ImVec4 BACKGROUND_COLOR;
extern const ImVec4 TEXT_COLOR;
extern const ImVec4 DISABLED_TEXT_COLOR;
extern const ImVec4 ACCENT_COLOR;
extern const ImVec4 STRONG_ACCENT_COLOR;
extern const ImVec4 GREY;
extern const ImVec4 LIGHT_GREY;
extern const ImVec4 DARK_ACCENT;
extern const ImVec4 BLACK;

}
