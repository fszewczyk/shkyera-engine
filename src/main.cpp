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
#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <ECS/Registry.hpp>

#include <UI/UI.hpp>

void addModel(std::shared_ptr<shkyera::Registry> registry, 
              const glm::vec3& position, 
              const std::string& name, 
              std::shared_ptr<shkyera::Mesh> mesh) {
    using namespace shkyera;

    auto entity = registry->addEntity();
    registry->addComponent<TransformComponent>(entity);
    registry->getComponent<TransformComponent>(entity).setPosition(position);
    registry->addComponent<NameComponent>(entity);
    registry->getComponent<NameComponent>(entity).setName(name);
    registry->addComponent<ModelComponent>(entity);
    registry->getComponent<ModelComponent>(entity).setMesh(mesh);
    registry->getComponent<ModelComponent>(entity).setMaterial(std::make_shared<Material>());
    registry->addComponent<BoxColliderComponent<ComponentMode::DEVELOPMENT>>(entity);
    registry->getComponent<BoxColliderComponent<ComponentMode::DEVELOPMENT>>(entity).box = mesh->getBoundingBox();
}

void addWireframe(std::shared_ptr<shkyera::Registry> registry, 
                  const glm::vec3& position, 
                  const std::string& name, 
                  std::shared_ptr<shkyera::Wireframe> wireframe) {
    using namespace shkyera;

    auto entity = registry->addEntity();
    registry->addComponent<TransformComponent>(entity);
    registry->getComponent<TransformComponent>(entity).setPosition(position);
    registry->addComponent<NameComponent>(entity);
    registry->getComponent<NameComponent>(entity).setName(name);
    registry->addComponent<WireframeComponent>(entity);
    registry->getComponent<WireframeComponent>(entity).setWireframe(wireframe);
}

void loadScene(std::shared_ptr<shkyera::Registry> registry) {
    using namespace shkyera;

    // Camera setup
    auto camera = registry->getCamera();
    registry->addComponent<TransformComponent>(camera);
    registry->addComponent<CameraComponent>(camera);

    // Add Cylinder and its wireframe
    addModel(registry, {3, 0, -3}, "Cylinder", std::shared_ptr<Mesh>(Mesh::Factory::createCylinder()));
    addWireframe(registry, {3, 3, -3}, "Cylinder Wireframe", std::shared_ptr<Wireframe>(Wireframe::Factory::createCylinder()));

    // Add Cube and its wireframe
    addModel(registry, {3, 0, 0}, "Cube", std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
    addWireframe(registry, {3, 3, 0}, "Cube Wireframe", std::shared_ptr<Wireframe>(Wireframe::Factory::createCube()));

    // Add Sphere and its wireframe
    addModel(registry, {3, 0, 3}, "Sphere", std::shared_ptr<Mesh>(Mesh::Factory::createSphere()));
    addWireframe(registry, {3, 3, 3}, "Sphere Wireframe", std::shared_ptr<Wireframe>(Wireframe::Factory::createSphere()));

    // Add Point Light
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
