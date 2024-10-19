#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <AssetManager/Mesh.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/UI.hpp>

void loadScene(std::shared_ptr<shkyera::Registry> registry) {
  using namespace shkyera;

  auto teapot = registry->addEntity();
  registry->addComponent<TransformComponent>(teapot);

  registry->addComponent<NameComponent>(teapot);
  registry->getComponent<NameComponent>(teapot).setName("Teapot");

  registry->addComponent<ModelComponent>(teapot);
  registry->getComponent<ModelComponent>(teapot).setMesh(AssetManager::getInstance().getAsset<Mesh>("resources/models/teapot.obj"));
  registry->getComponent<ModelComponent>(teapot).setMaterial(std::make_shared<Material>());

  auto pointLight = registry->addEntity();
  registry->addComponent<TransformComponent>(pointLight);
  registry->getComponent<TransformComponent>(pointLight).setPosition({-50, 50, 50});

  registry->addComponent<NameComponent>(pointLight);
  registry->getComponent<NameComponent>(pointLight).setName("Point Light");

  registry->addComponent<PointLightComponent>(pointLight);
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
