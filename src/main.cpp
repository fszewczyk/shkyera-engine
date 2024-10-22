#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <AssetManager/Mesh.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <ECS/Registry.hpp>

#include <UI/UI.hpp>

void loadScene(std::shared_ptr<shkyera::Registry> registry) {
  using namespace shkyera;

  auto camera = registry->getCamera();
  registry->addComponent<TransformComponent>(camera);
  registry->addComponent<CameraComponent>(camera);

  // Cylinder entity with its model
  auto cylinder = registry->addEntity();
  registry->addComponent<TransformComponent>(cylinder);
  registry->getComponent<TransformComponent>(cylinder).setPosition({3, 0, -3});
  registry->addComponent<NameComponent>(cylinder);
  registry->getComponent<NameComponent>(cylinder).setName("Cylinder");
  registry->addComponent<ModelComponent>(cylinder);
  registry->getComponent<ModelComponent>(cylinder).setMesh(std::shared_ptr<Mesh>(Mesh::Factory::createCylinder()));
  registry->getComponent<ModelComponent>(cylinder).setMaterial(std::make_shared<Material>());

  // Corresponding wireframe entity for Cylinder
  auto cylinderWireframe = registry->addEntity();
  registry->addComponent<TransformComponent>(cylinderWireframe);
  registry->getComponent<TransformComponent>(cylinderWireframe).setPosition({3, 3, -3}); // Offset by 2 units on x-axis
  registry->addComponent<NameComponent>(cylinderWireframe);
  registry->getComponent<NameComponent>(cylinderWireframe).setName("Cylinder Wireframe");
  registry->addComponent<WireframeComponent>(cylinderWireframe);
  registry->getComponent<WireframeComponent>(cylinderWireframe).setWireframe(std::shared_ptr<Wireframe>(Wireframe::Factory::createCylinder()));

  // Cube entity with its model
  auto cube = registry->addEntity();
  registry->addComponent<TransformComponent>(cube);
  registry->getComponent<TransformComponent>(cube).setPosition({3, 0, 0});
  registry->addComponent<NameComponent>(cube);
  registry->getComponent<NameComponent>(cube).setName("Cube");
  registry->addComponent<ModelComponent>(cube);
  registry->getComponent<ModelComponent>(cube).setMesh(std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
  registry->getComponent<ModelComponent>(cube).setMaterial(std::make_shared<Material>());

  // Corresponding wireframe entity for Cube
  auto cubeWireframe = registry->addEntity();
  registry->addComponent<TransformComponent>(cubeWireframe);
  registry->getComponent<TransformComponent>(cubeWireframe).setPosition({3, 3, 0}); // Offset by 2 units on x-axis
  registry->addComponent<NameComponent>(cubeWireframe);
  registry->getComponent<NameComponent>(cubeWireframe).setName("Cube Wireframe");
  registry->addComponent<WireframeComponent>(cubeWireframe);
  registry->getComponent<WireframeComponent>(cubeWireframe).setWireframe(std::shared_ptr<Wireframe>(Wireframe::Factory::createCube()));

  // Sphere entity with its model
  auto sphere = registry->addEntity();
  registry->addComponent<TransformComponent>(sphere);
  registry->getComponent<TransformComponent>(sphere).setPosition({3, 0, 3});
  registry->addComponent<NameComponent>(sphere);
  registry->getComponent<NameComponent>(sphere).setName("Sphere");
  registry->addComponent<ModelComponent>(sphere);
  registry->getComponent<ModelComponent>(sphere).setMesh(std::shared_ptr<Mesh>(Mesh::Factory::createSphere()));
  registry->getComponent<ModelComponent>(sphere).setMaterial(std::make_shared<Material>());

  // Corresponding wireframe entity for Sphere
  auto sphereWireframe = registry->addEntity();
  registry->addComponent<TransformComponent>(sphereWireframe);
  registry->getComponent<TransformComponent>(sphereWireframe).setPosition({3, 3, 3}); // Offset by 2 units on x-axis
  registry->addComponent<NameComponent>(sphereWireframe);
  registry->getComponent<NameComponent>(sphereWireframe).setName("Sphere Wireframe");
  registry->addComponent<WireframeComponent>(sphereWireframe);
  registry->getComponent<WireframeComponent>(sphereWireframe).setWireframe(std::shared_ptr<Wireframe>(Wireframe::Factory::createSphere()));

  // Point light entity
  auto pointLight = registry->addEntity();
  registry->addComponent<TransformComponent>(pointLight);
  registry->getComponent<TransformComponent>(pointLight).setPosition({-2, 5, 5});
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
