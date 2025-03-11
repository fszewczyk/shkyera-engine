#pragma once

#include <string_view>

#include <UI/Common/Style.hpp>

namespace shkyera::utils::ui {

void drawCenteredTitle(std::string_view text, ImFont* font = style::BIG_FONT);

}
