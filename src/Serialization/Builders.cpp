#include <Registry.pb.h>
#include <Components/NameComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Serialization/Builders.hpp>

namespace shkyera::serialization {

template <typename ProtoType>
using ProtoArray = const ::google::protobuf::RepeatedPtrField<ProtoType>;

template <typename ComponentType, typename ProtoType, typename ConversionFunc>
void loadComponents(Registry* registry, std::map<Entity, Entity>& protoToRegistryEntities, ProtoArray<ProtoType> protoComponents, ConversionFunc constructComponent) {
    for (const auto& entityAndComponent : protoComponents) {
        const auto& protoEntity = entityAndComponent.entity();
        if (!protoToRegistryEntities.contains(protoEntity)) {
            protoToRegistryEntities[protoEntity] = registry->addEntity();
        }
        const auto& entity = protoToRegistryEntities.at(protoEntity);
        registry->addComponent<ComponentType>(entity, constructComponent(entityAndComponent));
    }
}

std::unique_ptr<shkyera::Registry> fromBinary(std::istream& serialized) {
    shkyeraProto::Registry serializedRegistry;
    if (!serializedRegistry.ParseFromIstream(&serialized)) {
        return nullptr;
    }

    auto registry = std::make_unique<Registry>();
    std::map<Entity, Entity> protoToRegistryEntities;

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

    return registry;
}

void toBinary(std::ostream& outputStream, Registry const* registry) {
    shkyeraProto::Registry serializedRegistry;

    for (const auto& [entity, transform] : registry->getComponentSet<TransformComponent>()) {
        auto* protoTransform = serializedRegistry.add_transforms();
        protoTransform->set_entity(entity);
        protoTransform->mutable_position()->set_x(transform.getPosition().x);
        protoTransform->mutable_position()->set_y(transform.getPosition().y);
        protoTransform->mutable_position()->set_z(transform.getPosition().z);

        protoTransform->mutable_orientation()->set_x(transform.getOrientation().x);
        protoTransform->mutable_orientation()->set_y(transform.getOrientation().y);
        protoTransform->mutable_orientation()->set_z(transform.getOrientation().z);

        protoTransform->mutable_scale()->set_x(transform.getScale().x);
        protoTransform->mutable_scale()->set_y(transform.getScale().y);
        protoTransform->mutable_scale()->set_z(transform.getScale().z);
    }

    for (const auto& [entity, name] : registry->getComponentSet<NameComponent>()) {
        auto* protoName = serializedRegistry.add_names();
        protoName->set_entity(entity);
        protoName->set_name(name.getName());
    }

    serializedRegistry.SerializeToOstream(&outputStream);
}

}  // namespace shkyera::serialization
