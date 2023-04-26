#include <stdio.h>

#include "game/Game.hpp"
#include "ui/UI.hpp"

#include "imgui.h"

using namespace shkyera;

int main() {
    auto game = std::make_shared<Game>();
    UI ui(game);

    while (!ui.shouldClose())
        ui.draw();

    return 0;
}