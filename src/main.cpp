#include <stdio.h>

#include <chrono>
#include <iostream>
#include <thread>

#include <Utils/AssetUtils.hpp>
#include <AssetManager/Mesh.hpp>
#include <AssetManager/Image.hpp>
#include <Rendering/CubeMap.hpp>
#include <Common/Types.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/ParticleEmitterComponent.hpp>
#include <Components/BillboardComponent.hpp>
#include <Components/OverlayModelComponent.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/AmbientLightComponent.hpp>
#include <Components/SpotLightComponent.hpp>
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
    registry->getComponent<ModelComponent>(entity).mesh = HandleAndAsset<Mesh>{entity, mesh};
    registry->getComponent<ModelComponent>(entity).material = HandleAndAsset<Material>{std::nullopt, std::make_shared<Material>()};
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
    registry->getComponent<WireframeComponent>(entity).wireframe = HandleAndAsset<Wireframe>{entity, wireframe};

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

    auto fireplace = registry->addEntity();
    registry->addComponent<NameComponent>(fireplace).setName("Fireplace");
    registry->addComponent<TransformComponent>(fireplace).setPosition(glm::vec3{3, 0, 3});
    auto& fireParticleSystem = registry->addComponent<ParticleEmitterComponent>(fireplace);
    fireParticleSystem.emittedAtRadius = 1.0f;
    fireParticleSystem.emittedPerSecond = 30.0f;
    fireParticleSystem.gravity = 0.1f;
    fireParticleSystem.endTransparency = 0.1f;
    fireParticleSystem.endParticleSize = 0.0f;
    fireParticleSystem.initialVelocityDispersion = -0.5f;

    // Add Cylinder and its wireframe
    auto cylinderParent = registry->addEntity();
    registry->addComponent<TransformComponent>(cylinderParent);
    registry->getComponent<TransformComponent>(cylinderParent).setPosition({3, 0, -3});
    registry->addComponent<NameComponent>(cylinderParent);
    registry->getComponent<NameComponent>(cylinderParent).setName("Cylinder");
    auto cylinderModel = addModel(registry, {0, 0, 0}, "Cylinder Model", utils::assets::fromFactory<Mesh, &Mesh::Factory::createCylinder>(registry.get()));
    auto cylinderWireframe = addWireframe(registry, {0, 3, 0}, "Cylinder Wireframe", utils::assets::fromFactory<Wireframe, &Wireframe::Factory::createCylinder>(registry.get()));
    registry->getHierarchy().attributeChild(cylinderParent, cylinderModel);
    registry->getHierarchy().attributeChild(cylinderParent, cylinderWireframe);

    // Add Cube and its wireframe
    addModel(registry, {3, 0, 0}, "Cube", utils::assets::fromFactory<Mesh, &Mesh::Factory::createCube>(registry.get()));
    addWireframe(registry, {3, 3, 0}, "Cube Wireframe", utils::assets::fromFactory<Wireframe, &Wireframe::Factory::createCube>(registry.get()));

    // Add Sphere and its wireframe
    addModel(registry, {3, 6, 3}, "Sphere", utils::assets::fromFactory<Mesh, &Mesh::Factory::createSphere>(registry.get()));
    addWireframe(registry, {3, 9, 3}, "Sphere Wireframe", utils::assets::fromFactory<Wireframe, &Wireframe::Factory::createSphere>(registry.get()));

    // Add World Plane
    auto bunny = addModel(registry, {0, -1, 0}, "Bunny", utils::assets::readPermanent<Mesh>("resources/models/bunny.obj"));
    registry->getComponent<TransformComponent>(bunny).setScale({15, 15, 15});

    auto worldPlane = addModel(registry, {0, -2, 0}, "Plane", utils::assets::fromFactory<Mesh, &Mesh::Factory::createPlane>(registry.get()));
    registry->getComponent<TransformComponent>(worldPlane).setScale({15, 1, 15});

    // Add Point Light
    auto pointLight = registry->addEntity();
    registry->addComponent<TransformComponent>(pointLight);
    registry->getComponent<TransformComponent>(pointLight).setPosition({-2, 5, 1});
    registry->addComponent<NameComponent>(pointLight);
    registry->getComponent<NameComponent>(pointLight).setName("Point Light");
    registry->addComponent<PointLightComponent>(pointLight);
    registry->getComponent<PointLightComponent>(pointLight).range = 15;
    registry->getComponent<PointLightComponent>(pointLight).intensity = 1; 

    // Add Skybox
    auto sky = registry->addEntity();
    registry->addComponent<TransformComponent>(sky);
    registry->getComponent<TransformComponent>(sky).setOrientation({-M_PI_4, 0, -1.5 * M_PI_4});
    registry->addComponent<NameComponent>(sky);
    registry->getComponent<NameComponent>(sky).setName("Sun");
    registry->addComponent<DirectionalLightComponent>(sky);
    registry->getComponent<DirectionalLightComponent>(sky).color = glm::vec3{0.95, 0.95, 0.95};
    registry->getComponent<DirectionalLightComponent>(sky).intensity = 0.5;
    
    auto ambientLight = registry->addEntity();
    registry->addComponent<AmbientLightComponent>(ambientLight);
    registry->addComponent<NameComponent>(ambientLight).setName("Ambient Light");
    registry->addComponent<TransformComponent>(ambientLight).setPosition({1, 0, 0});

    auto redSpotLight = registry->addEntity();
    registry->addComponent<TransformComponent>(redSpotLight);
    registry->getComponent<TransformComponent>(redSpotLight).setOrientation({0, 1.5 * M_PI_2, -M_PI_4 / 2});
    registry->getComponent<TransformComponent>(redSpotLight).setPosition({20, 10, 20});
    registry->addComponent<NameComponent>(redSpotLight);
    registry->getComponent<NameComponent>(redSpotLight).setName("Red Spot Light");
    registry->addComponent<SpotLightComponent>(redSpotLight);
    registry->getComponent<SpotLightComponent>(redSpotLight).color = glm::vec3{0.95, 0.55, 0.15};
    registry->getComponent<SpotLightComponent>(redSpotLight).intensity = 2.0;
    registry->getComponent<SpotLightComponent>(redSpotLight).range = 50.0;
    registry->getComponent<SpotLightComponent>(redSpotLight).innerCutoff = glm::radians(10.0);
    registry->getComponent<SpotLightComponent>(redSpotLight).outerCutoff = glm::radians(15.0);

    auto blueSpotLight = registry->addEntity();
    registry->addComponent<TransformComponent>(blueSpotLight);
    registry->getComponent<TransformComponent>(blueSpotLight).setOrientation({0, M_PI_4, -M_PI_4 / 2});
    registry->getComponent<TransformComponent>(blueSpotLight).setPosition({-20, 10, 20});
    registry->addComponent<NameComponent>(blueSpotLight);
    registry->getComponent<NameComponent>(blueSpotLight).setName("Blue Spot Light");
    registry->addComponent<SpotLightComponent>(blueSpotLight);
    registry->getComponent<SpotLightComponent>(blueSpotLight).color = glm::vec3{0.15, 0.35, 0.95};
    registry->getComponent<SpotLightComponent>(blueSpotLight).intensity = 2.0;
    registry->getComponent<SpotLightComponent>(blueSpotLight).range = 50.0;
    registry->getComponent<SpotLightComponent>(blueSpotLight).innerCutoff = glm::radians(10.0);
    registry->getComponent<SpotLightComponent>(blueSpotLight).outerCutoff = glm::radians(15.0);

    const auto skyboxUp = utils::assets::addAndRead<Image>(registry.get(), []() { return Image("resources/skyboxes/day/py.png"); });
    const auto skyboxDown = utils::assets::addAndRead<Image>(registry.get(), []() { return Image("resources/skyboxes/day/ny.png"); });
    const auto skyboxLeft = utils::assets::addAndRead<Image>(registry.get(), []() { return Image("resources/skyboxes/day/nx.png"); });
    const auto skyboxRight = utils::assets::addAndRead<Image>(registry.get(), []() { return Image("resources/skyboxes/day/px.png"); });
    const auto skyboxFront = utils::assets::addAndRead<Image>(registry.get(), []() { return Image("resources/skyboxes/day/pz.png"); });
    const auto skyboxBack = utils::assets::addAndRead<Image>(registry.get(), []() { return Image("resources/skyboxes/day/nz.png"); });
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
