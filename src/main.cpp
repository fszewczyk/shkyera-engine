#include <stdio.h>

#include "ui/UI.hpp"

using namespace shkyera;

int main() {
    UI ui;

    while (!ui.shouldClose())
        ui.draw();

    return 0;
}