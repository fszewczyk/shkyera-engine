#include <Registry.pb.h>

#include <AssetManager/Asset.hpp>
#include <AssetManager/Material.hpp>
#include <AssetManager/Mesh.hpp>
#include <AssetManager/Texture.hpp>
#include <AssetManager/Wireframe.hpp>
#include <Common/Assert.hpp>
#include <Components/AmbientLightComponent.hpp>
#include <Components/AssetComponents/AssetComponent.hpp>
#include <Components/AssetComponents/AssetRoot.hpp>
#include <Components/BillboardComponent.hpp>
#include <Components/BoxColliderComponent.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/DirectionalLightComponent.hpp>
#include <Components/Environment.hpp>
#include <Components/ModelComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/ParticleEmitterComponent.hpp>
#include <Components/PointLightComponent.hpp>
#include <Components/PostProcessingVolumeComponent.hpp>
#include <Components/SceneCamera.hpp>
#include <Components/SkyboxComponent.hpp>
#include <Components/SpotLightComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/WireframeComponent.hpp>
#include <ECS/Entity.hpp>
#include <Math/AABB.hpp>
#include <Serialization/Builders.hpp>
#include <Utils/AssetLoaders.hpp>
#include <Utils/AssetUtils.hpp>
#include "Components/AssetComponents/DirectoryComponent.hpp"

namespace shkyera::serialization {

glm::vec3 fromProto(const shkyeraProto::Vec3& vec) {
    return glm::vec3{vec.x(), vec.y(), vec.z()};
}

template <typename AssetType>
HandleAndAsset<AssetType> fromProto(Registry* registry, const std::map<Entity, Entity>& protoToRegistryEntities, Entity protoEntity) {
    if (protoEntity == InvalidEntity) {
        return {};
    }

    SHKYERA_ASSERT(protoToRegistryEntities.contains(protoEntity), "Asset {} is not in the registry, so it cannot have {}", protoEntity, typeid(AssetType).name());

    const auto& registryAssetEntity = protoToRegistryEntities.at(protoEntity);
    SHKYERA_ASSERT(registry->hasComponent<AssetComponent<AssetType>>(registryAssetEntity), "Asset {} does not have the Asset Component ({})", protoEntity, typeid(AssetType).name());

    return {registryAssetEntity, utils::assets::read(registry->getComponent<AssetComponent<AssetType>>(registryAssetEntity))};
}

void toProto(const glm::vec3& vec, shkyeraProto::Vec3& out) {
    out.set_x(vec.x);
    out.set_y(vec.y);
    out.set_z(vec.z);
}

template <typename AssetType>
uint32_t toProto(const HandleAndAsset<AssetType>& asset) {
    return std::get<OptionalAssetHandle>(asset).value_or(0);
}

template <typename ProtoType>
using ProtoArray = ::google::protobuf::RepeatedPtrField<ProtoType>;

template <typename ComponentType, typename ProtoType, typename ConversionFunc>
void loadComponents(Registry* registry, std::map<Entity, Entity>& protoToRegistryEntities, const ProtoArray<ProtoType>& protoComponents, ConversionFunc constructComponent) {
    for (const auto& entityAndComponent : protoComponents) {
        const auto& protoEntity = entityAndComponent.entity();
        if (!protoToRegistryEntities.contains(protoEntity)) {
            protoToRegistryEntities[protoEntity] = registry->addEntity();
        }
        const auto& entity = protoToRegistryEntities.at(protoEntity);
        registry->addComponent<ComponentType>(entity, constructComponent(entityAndComponent));
    }
}

template <typename ComponentType, typename ProtoType, typename ConversionFunc>
void loadSingletonComponent(Registry* registry, std::map<Entity, Entity>& protoToRegistryEntities, const ProtoType& protoComponent, ConversionFunc constructComponent) {
    if (protoComponent.entity() == InvalidEntity) {
        return;
    }

    const auto& protoEntity = protoComponent.entity();
    if (!protoToRegistryEntities.contains(protoEntity)) {
        protoToRegistryEntities[protoEntity] = registry->addEntity();
    }
    const auto& entity = protoToRegistryEntities.at(protoEntity);

    SHKYERA_ASSERT(!registry->getEntity<ComponentType>().has_value(), "Registry already contains a SingletonComponent {} (Entity: {}, ProtoEntity: {}. Trying to override during deserialization is likely wrong", typeid(ComponentType).name(), entity, protoEntity);

    registry->assignComponent<ComponentType>(entity, constructComponent(protoComponent));
}

std::unique_ptr<shkyera::Registry> fromBinary(std::istream& serialized) {
    shkyeraProto::Registry serializedRegistry;
    if (!serializedRegistry.ParseFromIstream(&serialized)) {
        return nullptr;
    }

    auto registry = std::make_unique<Registry>();
    std::map<Entity, Entity> protoToRegistryEntities;

    // Assets
    loadComponents<AssetComponent<Image>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.images_path(),
        [registry = registry.get()](const auto& comp) {
            return AssetComponent<Image>(std::make_unique<utils::assets::PathAssetLoader<Image>>(comp.path()));
        });

    loadComponents<AssetComponent<Texture>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.textures_path(),
        [registry = registry.get()](const auto& comp) {
            return AssetComponent<Texture>(std::make_unique<utils::assets::PathAssetLoader<Texture>>(comp.path()));
        });

    loadComponents<AssetComponent<CubeMap>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.cubemaps(),
        [registry = registry.get(), &protoToRegistryEntities](const auto& comp) {
            return AssetComponent<CubeMap>(
                std::make_unique<utils::assets::CubeMapLoader>(
                    registry,
                    std::array<HandleAndAsset<Image>, 6>{
                        HandleAndAsset<Image>{protoToRegistryEntities.at(comp.up()), nullptr},
                        {protoToRegistryEntities.at(comp.down()), nullptr},
                        {protoToRegistryEntities.at(comp.left()), nullptr},
                        {protoToRegistryEntities.at(comp.right()), nullptr},
                        {protoToRegistryEntities.at(comp.front()), nullptr},
                        {protoToRegistryEntities.at(comp.back()), nullptr},
                    }));
        });

    loadComponents<AssetComponent<Mesh>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.meshes_path(),
        [](const auto& comp) {
            return AssetComponent<Mesh>(
                std::make_unique<utils::assets::PathAssetLoader<Mesh>>(comp.path()));
        });

    loadComponents<AssetComponent<Mesh>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.meshes_factory(),
        [](const auto& comp) {
            return AssetComponent<Mesh>(
                std::make_unique<utils::assets::FactoryAssetLoader<Mesh>>(
                    Mesh::Factory::Type(comp.type())));
        });

    loadComponents<AssetComponent<Wireframe>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.wireframes_path(),
        [](const auto& comp) {
            return AssetComponent<Wireframe>(
                std::make_unique<utils::assets::PathAssetLoader<Wireframe>>(comp.path()));
        });

    loadComponents<AssetComponent<Wireframe>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.wireframes_factory(),
        [](const auto& comp) {
            return AssetComponent<Wireframe>(
                std::make_unique<utils::assets::FactoryAssetLoader<Wireframe>>(
                    Wireframe::Factory::Type(comp.type())));
        });

    loadComponents<AssetComponent<Material>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.materials(),
        [registry = registry.get(), &protoToRegistryEntities](const auto& comp) {
            auto material = Material{};
            material.lit = comp.lit();
            material.alphaMultiplier = comp.alpha();
            material.emissiveStrength = comp.emissive_strength();
            material.normalMapStrength = comp.normal_strength();
            material.roughness = comp.roughness();
            material.metallic = comp.metallic();

            material.albedo = fromProto(comp.albedo());
            material.emissive = fromProto(comp.emissive());

            material.albedoTexture = fromProto<Texture>(registry, protoToRegistryEntities, comp.albedo_texture());
            material.normalTexture = fromProto<Texture>(registry, protoToRegistryEntities, comp.normal_texture());
            material.roughnessTexture = fromProto<Texture>(registry, protoToRegistryEntities, comp.roughness_texture());
            material.metallicTexture = fromProto<Texture>(registry, protoToRegistryEntities, comp.metallic_texture());
            material.emissiveTexture = fromProto<Texture>(registry, protoToRegistryEntities, comp.emissive_texture());

            return AssetComponent<Material>(
                std::make_unique<utils::assets::MaterialLoader>(registry, material));
        });

    // Singleton Components
    loadSingletonComponent<SceneCamera>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.scene_camera(),
        [](const auto&) { return SceneCamera{}; });

    loadSingletonComponent<Environment>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.environment(),
        [](const auto&) { return Environment{}; });

    loadSingletonComponent<AssetRoot>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.asset_root(),
        [](const auto&) { return AssetRoot{}; });

    // Components
    loadComponents<DirectoryComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.directories(),
        [](const auto& comp) { return DirectoryComponent(comp.name()); });

    loadComponents<NameComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.names(),
        [](const auto& comp) { return NameComponent(comp.name()); });

    loadComponents<TransformComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.transforms(),
        [](const auto& comp) {
            auto transform = TransformComponent{};
            const auto& pos = comp.position();
            const auto& ori = comp.orientation();
            const auto& scale = comp.scale();
            transform.setPosition({pos.x(), pos.y(), pos.z()});
            transform.setOrientation({ori.x(), ori.y(), ori.z()});
            transform.setScale({scale.x(), scale.y(), scale.z()});
            return transform;
        });

    loadComponents<CameraComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.cameras(),
        [](const auto& comp) {
            auto cameraComponent = CameraComponent{};
            cameraComponent.fov = comp.fov();
            cameraComponent.aspectRatio = comp.aspect_ratio();
            cameraComponent.nearPlane = comp.near_plane();
            cameraComponent.farPlane = comp.far_plane();
            cameraComponent.projectionType = static_cast<CameraComponent::ProjectionType>(comp.projection());
            return cameraComponent;
        });

    loadComponents<AmbientLightComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.ambient_lights(),
        [](const auto& comp) {
            return AmbientLightComponent{
                .intensity = comp.intensity(),
                .color = fromProto(comp.color())};
        });

    loadComponents<ModelComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.models(),
        [&protoToRegistryEntities, registry = registry.get()](const auto& comp) {
            auto modelComponent = ModelComponent();
            modelComponent.mesh = fromProto<Mesh>(registry, protoToRegistryEntities, comp.mesh());
            modelComponent.material = fromProto<Material>(registry, protoToRegistryEntities, comp.material());
            return modelComponent;
        });

    loadComponents<SkyboxComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.skyboxes(),
        [&protoToRegistryEntities, registry = registry.get()](const auto& comp) {
            return SkyboxComponent{
                .skyboxCubeMap = fromProto<CubeMap>(registry, protoToRegistryEntities, comp.cubemap())};
        });

    loadComponents<BoxColliderComponent<RuntimeMode::DEVELOPMENT>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.box_colliders_dev(),
        [](const auto& comp) {
            return BoxColliderComponent<RuntimeMode::DEVELOPMENT>{
                .box = AABB{fromProto(comp.center()), fromProto(comp.extents())}};
        });

    loadComponents<BoxColliderComponent<RuntimeMode::PRODUCTION>>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.box_colliders_prod(),
        [](const auto& comp) {
            return BoxColliderComponent<RuntimeMode::PRODUCTION>{
                .box = AABB{fromProto(comp.center()), fromProto(comp.extents())}};
        });

    loadComponents<DirectionalLightComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.directional_lights(),
        [](const auto& comp) {
            return DirectionalLightComponent{
                .intensity = comp.intensity(),
                .color = fromProto(comp.color())};
        });

    loadComponents<ParticleEmitterComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.particle_emitters(),
        [registry = registry.get(), &protoToRegistryEntities](const auto& comp) {
            ParticleEmitterComponent particleEmitterComponent{};
            particleEmitterComponent.enabled = comp.enabled();
            particleEmitterComponent.gravity = comp.gravity();
            particleEmitterComponent.emittedAtRadius = comp.emitted_at_radius();
            particleEmitterComponent.emittedPerSecond = comp.emitted_per_second();
            particleEmitterComponent.emittedPerSecondVariance = comp.emitted_per_second_variance();
            particleEmitterComponent.initialVelocity = comp.initial_velocity();
            particleEmitterComponent.initialVelocityVariance = comp.initial_velocity_variance();
            particleEmitterComponent.initialVelocityDispersion = comp.initial_velocity_dispersion();
            particleEmitterComponent.lifetime = comp.lifetime();
            particleEmitterComponent.lifetimeVariance = comp.lifetime_variance();
            particleEmitterComponent.initialParticleSize = comp.initial_particle_size();
            particleEmitterComponent.initialParticleSizeVariance = comp.initial_particle_size_variance();
            particleEmitterComponent.endParticleSize = comp.end_particle_size();
            particleEmitterComponent.endParticleSizeVariance = comp.end_particle_size_variance();
            particleEmitterComponent.material = fromProto<Material>(registry, protoToRegistryEntities, comp.material());
            particleEmitterComponent.initialTransparency = comp.initial_transparency();
            particleEmitterComponent.initialTransparencyVariance = comp.initial_transparency_variance();
            particleEmitterComponent.endTransparency = comp.end_transparency();
            particleEmitterComponent.endTransparencyVariance = comp.end_transparency_variance();
            return particleEmitterComponent;
        });

    loadComponents<PointLightComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.point_lights(),
        [](const auto& comp) {
            return PointLightComponent{
                .intensity = comp.intensity(),
                .range = comp.range(),
                .color = fromProto(comp.color())};
        });

    loadComponents<PostProcessingVolumeComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.post_processing_volumes(),
        [](const auto& comp) {
            return PostProcessingVolumeComponent{
                .global = comp.global(),
                .gamma = comp.gamma(),
                .toneMapping = comp.tone_mapping(),
                .antiAliasing = comp.anti_aliasing(),
                .bloomThreshold = comp.bloom_threshold(),
                .bloomWeight = comp.bloom_weight(),
                .ssaoStrength = comp.ssao_strength(),
                .ssaoRadius = comp.ssao_radius()};
        });

    loadComponents<SpotLightComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.spot_lights(),
        [](const auto& comp) {
            return SpotLightComponent{
                .intensity = comp.intensity(),
                .range = comp.range(),
                .innerCutoff = comp.inner_cutoff(),
                .outerCutoff = comp.outer_cutoff(),
                .color = fromProto(comp.color())};
        });

    loadComponents<WireframeComponent>(
        registry.get(),
        protoToRegistryEntities,
        serializedRegistry.wireframes(),
        [&protoToRegistryEntities, registry = registry.get()](const auto& comp) {
            WireframeComponent wireframeComponent{};
            wireframeComponent.wireframe = fromProto<Wireframe>(registry, protoToRegistryEntities, comp.wireframe());
            return wireframeComponent;
        });

    // This needs to be very last to make sure that protoEntity -> Entity mapping is complete
    for (const auto& relationship : serializedRegistry.ancestry_relations()) {
        SHKYERA_ASSERT(protoToRegistryEntities.contains(relationship.parent()), "Parent Proto Entity {} was not loaded. Cannot register a parent-child relationship with {}", relationship.parent(), relationship.child());
        SHKYERA_ASSERT(protoToRegistryEntities.contains(relationship.child()), "Child Proto Entity {} was not loaded. Cannot register a parent-child relationship with {}", relationship.child(), relationship.parent());
        SHKYERA_ASSERT(relationship.child() != relationship.parent(), "Parent and child cannot be equal ({})", relationship.child());

        const auto& parent = protoToRegistryEntities.at(relationship.parent());
        const auto& child = protoToRegistryEntities.at(relationship.child());
        registry->getHierarchy().attributeChild(parent, child);
    }

    return registry;
}

template <typename ComponentType, typename ProtoType, typename SerializeFunc>
void serializeComponents(Registry const* registry, ProtoArray<ProtoType>& protoComponents, SerializeFunc serializeFunc) {
    for (const auto& [entity, component] : registry->getComponentSet<ComponentType>()) {
        auto* protoComponent = protoComponents.Add();
        protoComponent->set_entity(entity);
        serializeFunc(component, *protoComponent);
    }
}

template <typename ComponentType, typename ProtoType, typename SerializeFunc>
void serializeSingletonComponent(Registry const* registry, ProtoType& protoComponent, SerializeFunc serializeFunc) {
    if (const auto entityOpt = registry->getEntity<ComponentType>()) {
        protoComponent.set_entity(*entityOpt);
        serializeFunc(registry->getComponent<ComponentType>(*entityOpt), protoComponent);
    }
}

void toBinary(std::ostream& outputStream, Registry const* registry) {
    shkyeraProto::Registry serializedRegistry;

    // Global Objects
    for (const auto& [parent, children] : registry->getHierarchy().getParentToChild()) {
        for (const auto& child : children) {
            auto* relation = serializedRegistry.add_ancestry_relations();
            relation->set_parent(parent);
            relation->set_child(child);
        }
    }

    // Assets
    for (const auto& [entity, mesh] : registry->getComponentSet<AssetComponent<Mesh>>()) {
        if (const auto* meshLoader = dynamic_cast<utils::assets::PathAssetLoader<Mesh>*>(mesh.constructionFunction.get())) {
            auto* protoMesh = serializedRegistry.add_meshes_path();
            protoMesh->set_entity(entity);
            protoMesh->set_path(meshLoader->path);
        } else if (const auto* meshLoader = dynamic_cast<utils::assets::FactoryAssetLoader<Mesh>*>(mesh.constructionFunction.get())) {
            auto* protoMesh = serializedRegistry.add_meshes_factory();
            protoMesh->set_entity(entity);
            protoMesh->set_type(static_cast<uint32_t>(meshLoader->type));
        } else {
            SHKYERA_ASSERT(false, "Mesh Asset {} cannot be serialized", entity);
            continue;
        }
    }

    serializeComponents<AssetComponent<Material>, shkyeraProto::MaterialAsset>(
        registry, *serializedRegistry.mutable_materials(),
        [](const AssetComponent<Material>& component, shkyeraProto::MaterialAsset& protoComponent) {
            if (const auto* materialLoader = dynamic_cast<utils::assets::MaterialLoader*>(component.constructionFunction.get())) {
                protoComponent.set_lit(materialLoader->mat.lit);
                protoComponent.set_alpha(materialLoader->mat.alphaMultiplier);
                protoComponent.set_emissive_strength(materialLoader->mat.emissiveStrength);
                protoComponent.set_normal_strength(materialLoader->mat.normalMapStrength);
                protoComponent.set_roughness(materialLoader->mat.roughness);
                protoComponent.set_metallic(materialLoader->mat.metallic);

                toProto(materialLoader->mat.albedo, *protoComponent.mutable_albedo());
                toProto(materialLoader->mat.emissive, *protoComponent.mutable_emissive());

                protoComponent.set_albedo_texture(toProto(materialLoader->mat.albedoTexture));
                protoComponent.set_normal_texture(toProto(materialLoader->mat.normalTexture));
                protoComponent.set_roughness_texture(toProto(materialLoader->mat.roughnessTexture));
                protoComponent.set_metallic_texture(toProto(materialLoader->mat.metallicTexture));
                protoComponent.set_emissive_texture(toProto(materialLoader->mat.emissiveTexture));
            } else {
                SHKYERA_ASSERT(false, "Material Asset cannot be serialized");
            }
        });

    for (const auto& [entity, wireframe] : registry->getComponentSet<AssetComponent<Wireframe>>()) {
        if (const auto* wireframeLoader = dynamic_cast<utils::assets::PathAssetLoader<Wireframe>*>(wireframe.constructionFunction.get())) {
            auto* protoWireframe = serializedRegistry.add_wireframes_path();
            protoWireframe->set_entity(entity);
            protoWireframe->set_path(wireframeLoader->path);
        } else if (const auto* wireframeLoader = dynamic_cast<utils::assets::FactoryAssetLoader<Wireframe>*>(wireframe.constructionFunction.get())) {
            auto* protoWireframe = serializedRegistry.add_wireframes_factory();
            protoWireframe->set_entity(entity);
            protoWireframe->set_type(static_cast<uint32_t>(wireframeLoader->type));
        } else {
            SHKYERA_ASSERT(false, "Wireframe Asset {} cannot be serialized", entity);
            continue;
        }
    }

    for (const auto& [entity, image] : registry->getComponentSet<AssetComponent<Image>>()) {
        if (const auto* imageLoader = dynamic_cast<utils::assets::PathAssetLoader<Image>*>(image.constructionFunction.get())) {
            auto* protoImage = serializedRegistry.add_images_path();
            protoImage->set_entity(entity);
            protoImage->set_path(imageLoader->path);
        } else {
            SHKYERA_ASSERT(false, "Image Asset {} cannot be serialized", entity);
            continue;
        }
    }

    for (const auto& [entity, texture] : registry->getComponentSet<AssetComponent<Texture>>()) {
        if (const auto* textureLoader = dynamic_cast<utils::assets::PathAssetLoader<Texture>*>(texture.constructionFunction.get())) {
            auto* protoTexture = serializedRegistry.add_textures_path();
            protoTexture->set_entity(entity);
            protoTexture->set_path(textureLoader->path);
        } else {
            SHKYERA_ASSERT(false, "Texture Asset {} cannot be serialized", entity);
            continue;
        }
    }

    for (const auto& [entity, cubemap] : registry->getComponentSet<AssetComponent<CubeMap>>()) {
        if (const auto* cubemapLoader = dynamic_cast<utils::assets::CubeMapLoader*>(cubemap.constructionFunction.get())) {
            auto* protoCubemap = serializedRegistry.add_cubemaps();
            protoCubemap->set_entity(entity);
            protoCubemap->set_up(toProto(cubemapLoader->faces[0]));
            protoCubemap->set_down(toProto(cubemapLoader->faces[1]));
            protoCubemap->set_left(toProto(cubemapLoader->faces[2]));
            protoCubemap->set_right(toProto(cubemapLoader->faces[3]));
            protoCubemap->set_front(toProto(cubemapLoader->faces[4]));
            protoCubemap->set_back(toProto(cubemapLoader->faces[5]));
        } else {
            SHKYERA_ASSERT(false, "CubeMap Asset {} cannot be serialized", entity);
            continue;
        }
    }

    // Singleton Components
    serializeSingletonComponent<SceneCamera>(
        registry, *serializedRegistry.mutable_scene_camera(),
        [](const auto&, const auto&) {});

    serializeSingletonComponent<Environment>(
        registry, *serializedRegistry.mutable_environment(),
        [](const auto&, const auto&) {});

    serializeSingletonComponent<AssetRoot>(
        registry, *serializedRegistry.mutable_asset_root(),
        [](const auto&, const auto&) {});

    // Components
    serializeComponents<DirectoryComponent>(
        registry, *serializedRegistry.mutable_directories(),
        [](const auto& component, auto& protoComponent) {
            protoComponent.set_name(component.name);
        });

    serializeComponents<CameraComponent, shkyeraProto::CameraComponent>(
        registry, *serializedRegistry.mutable_cameras(),
        [](const CameraComponent& component, shkyeraProto::CameraComponent& protoComponent) {
            protoComponent.set_fov(component.fov);
            protoComponent.set_aspect_ratio(component.aspectRatio);
            protoComponent.set_near_plane(component.nearPlane);
            protoComponent.set_far_plane(component.farPlane);
            protoComponent.set_projection(static_cast<uint32_t>(component.projectionType));
        });

    serializeComponents<TransformComponent, shkyeraProto::TransformComponent>(
        registry, *serializedRegistry.mutable_transforms(),
        [](const TransformComponent& component, shkyeraProto::TransformComponent& protoComponent) {
            toProto(component.getPosition(), *protoComponent.mutable_position());
            toProto(component.getOrientation(), *protoComponent.mutable_orientation());
            toProto(component.getScale(), *protoComponent.mutable_scale());
        });

    serializeComponents<NameComponent, shkyeraProto::NameComponent>(
        registry, *serializedRegistry.mutable_names(),
        [](const NameComponent& component, shkyeraProto::NameComponent& protoComponent) {
            protoComponent.set_name(component.getName());
        });

    serializeComponents<AmbientLightComponent, shkyeraProto::AmbientLightComponent>(
        registry, *serializedRegistry.mutable_ambient_lights(),
        [](const AmbientLightComponent& component, shkyeraProto::AmbientLightComponent& protoComponent) {
            protoComponent.set_intensity(component.intensity);
            toProto(component.color, *protoComponent.mutable_color());
        });

    serializeComponents<ModelComponent, shkyeraProto::ModelComponent>(
        registry, *serializedRegistry.mutable_models(),
        [](const ModelComponent& component, shkyeraProto::ModelComponent& protoComponent) {
            protoComponent.set_mesh(toProto(component.mesh));
            protoComponent.set_material(toProto(component.material));
        });

    serializeComponents<SkyboxComponent>(
        registry, *serializedRegistry.mutable_skyboxes(),
        [](const auto& component, auto& protoComponent) {
            protoComponent.set_cubemap(toProto(component.skyboxCubeMap));
        });

    const auto serializeBillboards = [&](const auto& component, auto& protoComponent) {
        protoComponent.set_orientation(static_cast<uint32_t>(component.orientation));
        protoComponent.set_scale(static_cast<uint32_t>(component.scale));
        protoComponent.set_occlusion(static_cast<uint32_t>(component.occlusion));
        protoComponent.set_material(toProto(component.material));
    };
    serializeComponents<BillboardComponent<RuntimeMode::DEVELOPMENT>, shkyeraProto::BillboardComponent>(
        registry, *serializedRegistry.mutable_billboards_dev(), serializeBillboards);

    serializeComponents<BillboardComponent<RuntimeMode::PRODUCTION>, shkyeraProto::BillboardComponent>(
        registry, *serializedRegistry.mutable_billboards_dev(), serializeBillboards);

    const auto serializeBoxes = [&](const auto& component, auto& protoComponent) {
        toProto(component.box.center, *protoComponent.mutable_center());
        toProto(component.box.extents, *protoComponent.mutable_extents());
    };
    serializeComponents<BoxColliderComponent<RuntimeMode::DEVELOPMENT>, shkyeraProto::BoxColliderComponent>(
        registry, *serializedRegistry.mutable_box_colliders_dev(), serializeBoxes);
    serializeComponents<BoxColliderComponent<RuntimeMode::PRODUCTION>, shkyeraProto::BoxColliderComponent>(
        registry, *serializedRegistry.mutable_box_colliders_prod(), serializeBoxes);

    serializeComponents<DirectionalLightComponent, shkyeraProto::DirectionalLightComponent>(
        registry, *serializedRegistry.mutable_directional_lights(),
        [](const DirectionalLightComponent& component, shkyeraProto::DirectionalLightComponent& protoComponent) {
            protoComponent.set_intensity(component.intensity);
            toProto(component.color, *protoComponent.mutable_color());
        });

    serializeComponents<ParticleEmitterComponent, shkyeraProto::ParticleEmitterComponent>(
        registry, *serializedRegistry.mutable_particle_emitters(),
        [](const ParticleEmitterComponent& component, shkyeraProto::ParticleEmitterComponent& protoComponent) {
            protoComponent.set_enabled(component.enabled);
            protoComponent.set_gravity(component.gravity);
            protoComponent.set_emitted_at_radius(component.emittedAtRadius);
            protoComponent.set_emitted_per_second(component.emittedPerSecond);
            protoComponent.set_emitted_per_second_variance(component.emittedPerSecondVariance);
            protoComponent.set_initial_velocity(component.initialVelocity);
            protoComponent.set_initial_velocity_variance(component.initialVelocityVariance);
            protoComponent.set_initial_velocity_dispersion(component.initialVelocityDispersion);
            protoComponent.set_lifetime(component.lifetime);
            protoComponent.set_lifetime_variance(component.lifetimeVariance);
            protoComponent.set_initial_particle_size(component.initialParticleSize);
            protoComponent.set_initial_particle_size_variance(component.initialParticleSizeVariance);
            protoComponent.set_end_particle_size(component.endParticleSize);
            protoComponent.set_end_particle_size_variance(component.endParticleSizeVariance);
            protoComponent.set_material(toProto(component.material));
            protoComponent.set_initial_transparency(component.initialTransparency);
            protoComponent.set_initial_transparency_variance(component.initialTransparencyVariance);
            protoComponent.set_end_transparency(component.endTransparency);
            protoComponent.set_end_transparency_variance(component.endTransparencyVariance);
        });

    serializeComponents<PointLightComponent, shkyeraProto::PointLightComponent>(
        registry, *serializedRegistry.mutable_point_lights(),
        [](const PointLightComponent& component, shkyeraProto::PointLightComponent& protoComponent) {
            protoComponent.set_intensity(component.intensity);
            protoComponent.set_range(component.range);
            toProto(component.color, *protoComponent.mutable_color());
        });

    serializeComponents<PostProcessingVolumeComponent, shkyeraProto::PostProcessingVolumeComponent>(
        registry, *serializedRegistry.mutable_post_processing_volumes(),
        [](const PostProcessingVolumeComponent& component, shkyeraProto::PostProcessingVolumeComponent& protoComponent) {
            protoComponent.set_global(component.global);
            protoComponent.set_gamma(component.gamma);
            protoComponent.set_tone_mapping(component.toneMapping);
            protoComponent.set_anti_aliasing(component.antiAliasing);
            protoComponent.set_bloom_threshold(component.bloomThreshold);
            protoComponent.set_bloom_weight(component.bloomWeight);
            protoComponent.set_ssao_strength(component.ssaoStrength);
            protoComponent.set_ssao_radius(component.ssaoRadius);
        });

    serializeComponents<SpotLightComponent, shkyeraProto::SpotLightComponent>(
        registry, *serializedRegistry.mutable_spot_lights(),
        [](const SpotLightComponent& component, shkyeraProto::SpotLightComponent& protoComponent) {
            protoComponent.set_intensity(component.intensity);
            protoComponent.set_range(component.range);
            protoComponent.set_inner_cutoff(component.innerCutoff);
            protoComponent.set_outer_cutoff(component.outerCutoff);
            toProto(component.color, *protoComponent.mutable_color());
        });

    serializeComponents<WireframeComponent, shkyeraProto::WireframeComponent>(
        registry, *serializedRegistry.mutable_wireframes(),
        [](const WireframeComponent& component, shkyeraProto::WireframeComponent& protoComponent) {
            protoComponent.set_wireframe(toProto(component.wireframe));
        });

    serializedRegistry.SerializeToOstream(&outputStream);
}

}  // namespace shkyera::serialization
