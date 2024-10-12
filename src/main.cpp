#include <stdio.h>

#include <iostream>
#include <thread>
#include <chrono>

#include <ECS/Registry.hpp>
#include <UI/UI.hpp>

int main() {
    using namespace shkyera;

    auto registry = std::make_shared<Registry>();
    auto ui = UI(registry);

    while(!ui.shouldClose())
    {
        ui.draw();
    }

    return 0;
}
