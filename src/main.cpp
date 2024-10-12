#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <ECS/Registry.hpp>
#include <UI/UI.hpp>

int main() {
  using namespace shkyera;

  auto registry = std::make_shared<Registry>();
  auto ui = UI(registry);

  while (!ui.shouldClose()) {
    ui.draw();
  }

  return 0;
}
