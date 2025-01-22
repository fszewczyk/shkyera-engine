#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <AssetManager/Mesh.hpp>
#include <AssetManager/Image.hpp>
#include <AssetManager/AssetManager.hpp>
#include <Rendering/CubeMap.hpp>
#include <Common/Types.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/OverlayModelComponent.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/AmbientLightComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <Components/DirectionalLightComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/UI.hpp>

static shkyera::Entity addModel(std::shared_ptr<shkyera::Registry> registry, 
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
    registry->addComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(entity);
    registry->getComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(entity).box = mesh->getBoundingBox();

    return entity;
}

shkyera::Entity addWireframe(std::shared_ptr<shkyera::Registry> registry, 
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

    return entity;
}

void loadScene(std::shared_ptr<shkyera::Registry> registry) {
    using namespace shkyera;

    // Camera setup
    auto camera = registry->getCamera();
    registry->addComponent<TransformComponent>(camera);
    registry->getComponent<TransformComponent>(camera).setPosition({0, 15, 0});
    registry->getComponent<TransformComponent>(camera).setOrientation({-M_PI_2 + 0.01, 0, 0});
    registry->addComponent<CameraComponent>(camera);

    // Add Cylinder and its wireframe
    auto cylinderParent = registry->addEntity();
    registry->addComponent<TransformComponent>(cylinderParent);
    registry->getComponent<TransformComponent>(cylinderParent).setPosition({3, 0, -3});
    registry->addComponent<NameComponent>(cylinderParent);
    registry->getComponent<NameComponent>(cylinderParent).setName("Cylinder");
    auto cylinderModel = addModel(registry, {0, 0, 0}, "Cylinder Model", std::shared_ptr<Mesh>(Mesh::Factory::createCylinder()));
    auto cylinderWireframe = addWireframe(registry, {0, 3, 0}, "Cylinder Wireframe", std::shared_ptr<Wireframe>(Wireframe::Factory::createCylinder()));
    registry->getHierarchy().attributeChild(cylinderParent, cylinderModel);
    registry->getHierarchy().attributeChild(cylinderParent, cylinderWireframe);

    // Add Cube and its wireframe
    addModel(registry, {3, 0, 0}, "Cube", std::shared_ptr<Mesh>(Mesh::Factory::createCube()));
    addWireframe(registry, {3, 3, 0}, "Cube Wireframe", std::shared_ptr<Wireframe>(Wireframe::Factory::createCube()));

    // Add Sphere and its wireframe
    addModel(registry, {3, 6, 3}, "Sphere", std::shared_ptr<Mesh>(Mesh::Factory::createSphere()));
    addWireframe(registry, {3, 9, 3}, "Sphere Wireframe", std::shared_ptr<Wireframe>(Wireframe::Factory::createSphere()));

    // Add World Plane
    auto worldPlane = addModel(registry, {0, -2, 0}, "Plane", std::shared_ptr<Mesh>(Mesh::Factory::createPlane()));
    registry->getComponent<TransformComponent>(worldPlane).setScale({15, 1, 15});

    // Add Point Light
    auto pointLight = registry->addEntity();
    registry->addComponent<TransformComponent>(pointLight);
    registry->getComponent<TransformComponent>(pointLight).setPosition({4.7, 8.1, 2});
    registry->addComponent<NameComponent>(pointLight);
    registry->getComponent<NameComponent>(pointLight).setName("Point Light");
    registry->addComponent<PointLightComponent>(pointLight);
    registry->getComponent<PointLightComponent>(pointLight).range = 15;
    registry->getComponent<PointLightComponent>(pointLight).intensity = 3; 

    // Add Skybox
    auto sky = registry->addEntity();
    registry->addComponent<TransformComponent>(sky);
    registry->getComponent<TransformComponent>(sky).setOrientation({-M_PI_2 + 0.1, -0.1, 0});
    registry->addComponent<NameComponent>(sky);
    registry->getComponent<NameComponent>(sky).setName("Sun");
    registry->addComponent<DirectionalLightComponent>(sky);
    registry->getComponent<DirectionalLightComponent>(sky).color = glm::vec3{0.95, 0.95, 0.95};
    registry->getComponent<DirectionalLightComponent>(sky).intensity = 0.4;
    registry->addComponent<AmbientLightComponent>(sky);

    auto moon = registry->addEntity();
    registry->addComponent<TransformComponent>(moon);
    registry->getComponent<TransformComponent>(moon).setOrientation({-M_PI_2 / 2, -M_PI_2 / 2, 0});
    registry->addComponent<NameComponent>(moon);
    registry->getComponent<NameComponent>(moon).setName("Moon");
    registry->addComponent<DirectionalLightComponent>(moon);
    registry->getComponent<DirectionalLightComponent>(moon).color = glm::vec3{0.85, 0.95, 0.95};
    registry->getComponent<DirectionalLightComponent>(moon).intensity = 0.15;
    registry->addComponent<AmbientLightComponent>(moon);

    const auto skyboxUp = AssetManager::getInstance().getAsset<Image>("resources/skyboxes/day/py.png");
    const auto skyboxDown = AssetManager::getInstance().getAsset<Image>("resources/skyboxes/day/ny.png");
    const auto skyboxLeft = AssetManager::getInstance().getAsset<Image>("resources/skyboxes/day/nx.png");
    const auto skyboxRight = AssetManager::getInstance().getAsset<Image>("resources/skyboxes/day/px.png");
    const auto skyboxFront = AssetManager::getInstance().getAsset<Image>("resources/skyboxes/day/pz.png");
    const auto skyboxBack = AssetManager::getInstance().getAsset<Image>("resources/skyboxes/day/nz.png");
    registry->addComponent<SkyboxComponent>(registry->getEnvironment());
    registry->getComponent<SkyboxComponent>(registry->getEnvironment()).skyboxCubeMap.loadFaces(skyboxUp, skyboxDown, skyboxLeft, skyboxRight, skyboxFront, skyboxBack);
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
