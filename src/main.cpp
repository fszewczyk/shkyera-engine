#include <stdio.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <ECS/Registry.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/LoggingComponent.hpp>

using namespace shkyera;

void gameLoop(shkyera::Registry& registry) {
    while (true) {
        for (auto& component : registry.getComponentSet<TransformComponent>().getComponents()) {
            component.update();
        }
        for (auto& component : registry.getComponentSet<LoggingComponent>().getComponents()) {
            component.update();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    Registry registry;

    auto entity1 = registry.addEntity();
    registry.addComponent<shkyera::TransformComponent>(entity1);  // Add Transform Component
    registry.getComponent<shkyera::TransformComponent>(entity1).setPosition({1.0f, 2.0f, 0.0f});
    
    auto entity2 = registry.addEntity();
    registry.addComponent<shkyera::TransformComponent>(entity2);  // Add Transform Component
    registry.getComponent<shkyera::TransformComponent>(entity2).setPosition({3.0f, 4.0f, 0.0f});
    
    auto entity3 = registry.addEntity();
    registry.addComponent<shkyera::TransformComponent>(entity3);
    registry.addComponent<shkyera::LoggingComponent>(entity3);  // Add Logging Component
    registry.getComponent<shkyera::TransformComponent>(entity3).setPosition({5.0f, 6.0f, 0.0f});
    registry.getComponent<shkyera::LoggingComponent>(entity3).log("Entity 3 created!");

    std::thread gameThread(gameLoop, std::ref(registry));

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    gameThread.join();

    return 0;
}