#include <stdio.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include <AssetManager/CubeMap.hpp>
#include <AssetManager/Image.hpp>
#include <AssetManager/Mesh.hpp>
#include <Common/Types.hpp>
#include <Components/AmbientLightComponent.hpp>
#include <Components/AssetComponents/AssetRoot.hpp>
#include <Components/BillboardComponent.hpp>
#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/DirectionalLightComponent.hpp>
#include <Components/Environment.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/OverlayModelComponent.hpp>
#include <Components/ParticleEmitterComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <Components/PostProcessingVolumeComponent.hpp>
#include <Components/SceneCamera.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/SpotLightComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <ECS/Registry.hpp>
#include <Serialization/Builders.hpp>
#include <UI/UI.hpp>
#include <Utils/AssetUtils.hpp>
#include "AssetManager/Asset.hpp"
#include "Utils/AssetLoaders.hpp"

static shkyera::Entity addModel(std::shared_ptr<shkyera::Registry> registry, const glm::vec3& position,
                                const std::string& name, shkyera::HandleAndAsset<shkyera::Mesh> mesh) {
  using namespace shkyera;

  auto entity = registry->addEntity();
  registry->addComponent<TransformComponent>(entity);
  registry->getComponent<TransformComponent>(entity).setPosition(position);
  registry->addComponent<NameComponent>(entity);
  registry->getComponent<NameComponent>(entity).setName(name);
  registry->addComponent<ModelComponent>(entity);
  registry->getComponent<ModelComponent>(entity).mesh = mesh;
  registry->getComponent<ModelComponent>(entity).material = utils::assets::add<Material>(
      registry.get(), std::make_unique<utils::assets::MaterialLoader>(registry.get(), Material{}));
  registry->addComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(entity);
  registry->getComponent<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(entity).box =
      std::get<AssetRef<Mesh>>(mesh)->getBoundingBox();

  return entity;
}

shkyera::Entity addWireframe(std::shared_ptr<shkyera::Registry> registry, const glm::vec3& position,
                             const std::string& name, shkyera::HandleAndAsset<shkyera::Wireframe> wireframe) {
  using namespace shkyera;

  auto entity = registry->addEntity();
  registry->addComponent<TransformComponent>(entity);
  registry->getComponent<TransformComponent>(entity).setPosition(position);
  registry->addComponent<NameComponent>(entity);
  registry->getComponent<NameComponent>(entity).setName(name);
  registry->addComponent<WireframeComponent>(entity);
  registry->getComponent<WireframeComponent>(entity).wireframe = wireframe;

  return entity;
}

void loadScene(std::shared_ptr<shkyera::Registry> registry) {
  using namespace shkyera;

  auto rootHandle = *utils::assets::registerAll("resources", registry.get());
  registry->assignComponent<AssetRoot>(rootHandle);

  // Camera setup
  const auto camera = registry->addEntity();
  registry->assignComponent<SceneCamera>(camera);
  registry->addComponent<TransformComponent>(camera);
  registry->getComponent<TransformComponent>(camera).setPosition({0, 15, 0});
  registry->getComponent<TransformComponent>(camera).setOrientation({-M_PI_2 + 0.01, 0, 0});
  registry->addComponent<CameraComponent>(camera);

  auto fireplace = registry->addEntity();
  registry->addComponent<NameComponent>(fireplace).setName("Fireplace");
  registry->addComponent<TransformComponent>(fireplace).setPosition(glm::vec3{3, 0, 3});
  auto& fireplaceEmitter = registry->addComponent<ParticleEmitterComponent>(fireplace);

  auto postProcessingVol = registry->addEntity();
  registry->addComponent<NameComponent>(postProcessingVol).setName("Post-Processing");
  registry->addComponent<TransformComponent>(postProcessingVol).setPosition(glm::vec3{-3, 0, -3});
  registry->addComponent<TransformComponent>(postProcessingVol).setScale(glm::vec3{3, 1, 2});
  registry->addComponent<PostProcessingVolumeComponent>(postProcessingVol);

  // Add Cylinder and its wireframe
  auto cylinderParent = registry->addEntity();
  registry->addComponent<TransformComponent>(cylinderParent);
  registry->getComponent<TransformComponent>(cylinderParent).setPosition({3, 0, -3});
  registry->addComponent<NameComponent>(cylinderParent);
  registry->getComponent<NameComponent>(cylinderParent).setName("Cylinder");
  auto cylinderModel = addModel(registry, {0, 0, 0}, "Cylinder Model",
                                utils::assets::add<Mesh>(registry.get(), Mesh::Factory::Type::CYLINDER));
  auto cylinderWireframe =
      addWireframe(registry, {0, 3, 0}, "Cylinder Wireframe",
                   utils::assets::add<Wireframe>(registry.get(), Wireframe::Factory::Type::CYLINDER));
  registry->getHierarchy().attributeChild(cylinderParent, cylinderModel);
  registry->getHierarchy().attributeChild(cylinderParent, cylinderWireframe);

  // Add Cube and its wireframe
  addModel(registry, {3, 0, 0}, "Cube", utils::assets::add<Mesh>(registry.get(), Mesh::Factory::Type::CUBE));
  addWireframe(registry, {3, 3, 0}, "Cube Wireframe",
               utils::assets::add<Wireframe>(registry.get(), Wireframe::Factory::Type::CUBE));

  // Add Sphere and its wireframe
  addModel(registry, {3, 6, 3}, "Sphere", utils::assets::add<Mesh>(registry.get(), Mesh::Factory::Type::SPHERE));
  addWireframe(registry, {3, 9, 3}, "Sphere Wireframe",
               utils::assets::add<Wireframe>(registry.get(), Wireframe::Factory::Type::SPHERE));

  // Add World Plane
  auto bunny =
      addModel(registry, {0, -1, 0}, "Bunny", utils::assets::add<Mesh>(registry.get(), "resources/models/bunny.obj"));
  registry->getComponent<TransformComponent>(bunny).setScale({15, 15, 15});

  auto worldPlane =
      addModel(registry, {0, -2, 0}, "Plane", utils::assets::add<Mesh>(registry.get(), Mesh::Factory::Type::PLANE));
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

  const auto skyboxUp = utils::assets::add<Image>(registry.get(), "resources/skyboxes/day/py.png");
  const auto skyboxDown = utils::assets::add<Image>(registry.get(), "resources/skyboxes/day/ny.png");
  const auto skyboxLeft = utils::assets::add<Image>(registry.get(), "resources/skyboxes/day/nx.png");
  const auto skyboxRight = utils::assets::add<Image>(registry.get(), "resources/skyboxes/day/px.png");
  const auto skyboxFront = utils::assets::add<Image>(registry.get(), "resources/skyboxes/day/pz.png");
  const auto skyboxBack = utils::assets::add<Image>(registry.get(), "resources/skyboxes/day/nz.png");
  const auto environment = registry->addEntity();
  registry->assignComponent<Environment>(environment);
  registry->addComponent<SkyboxComponent>(environment);
  registry->getComponent<SkyboxComponent>(environment).skyboxCubeMap = utils::assets::add<CubeMap>(
      registry.get(),
      std::make_unique<utils::assets::CubeMapLoader>(
          registry.get(), std::array{skyboxUp, skyboxDown, skyboxLeft, skyboxRight, skyboxFront, skyboxBack}));

  Material mat{};
  mat.albedoTexture = utils::assets::add<Texture>(registry.get(), "resources/skyboxes/day/py.png");
  fireplaceEmitter.material = utils::assets::add<Material>(
      registry.get(), std::make_unique<utils::assets::MaterialLoader>(registry.get(), mat));
  registry->addComponent<NameComponent>(*std::get<OptionalAssetHandle>(fireplaceEmitter.material))
      .setName("Hardloaded Material");
  registry->getHierarchy().attributeChild(*registry->getEntity<AssetRoot>(),
                                          *std::get<OptionalAssetHandle>(fireplaceEmitter.material));
}

int main() {
  using namespace shkyera;

  auto ui = UI();
  auto registry = std::make_shared<Registry>();
  loadScene(registry);

  std::stringstream registryBuffer;
  serialization::toBinary(registryBuffer, registry.get());

  std::shared_ptr<Registry> deserializedRegistry = serialization::fromBinary(registryBuffer);

  ui.initialize(deserializedRegistry);

  while (!ui.shouldClose()) {
    ui.draw();
  }

  return 0;
}
