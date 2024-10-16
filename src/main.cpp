#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <AssetManager/Mesh.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/MeshComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/UI.hpp>

void loadScene(std::shared_ptr<shkyera::Registry> registry) {
  using namespace shkyera;

  auto teapot = registry->addEntity();
  registry->addComponent<TransformComponent>(teapot);

  registry->addComponent<NameComponent>(teapot);
  registry->getComponent<NameComponent>(teapot).setName("Teapot :)");

  auto teapotMesh = AssetManager::getInstance().getAsset<Mesh>("resources/models/teapot.obj");
  registry->addComponent<MeshComponent>(teapot);
  registry->getComponent<MeshComponent>(teapot).setMesh(teapotMesh);
}

int main() {
  using namespace shkyera;

  auto registry = std::make_shared<Registry>();
  auto ui = UI(registry);
  
  loadScene(registry);

  while (!ui.shouldClose()) {
    ui.draw();
  }
  return 0;
}
