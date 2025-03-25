#pragma once

#include <AssetManager/Material.hpp>
#include <AssetManager/Mesh.hpp>
#include <Common/Serialization.hpp>
#include <Components/BaseComponent.hpp>
#include "cereal/cereal.hpp"

namespace shkyera {

class ModelComponent : public BaseComponent<ModelComponent> {
   public:
    HandleAndAsset<Mesh> mesh{};
    HandleAndAsset<Material> material{};

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(mesh));
        archive(CEREAL_NVP(material));
    }

    void updateImpl() const {
        const auto& meshAsset = std::get<AssetRef<Mesh>>(mesh);
        if (meshAsset) {
            meshAsset->bind();
            glDrawElements(GL_TRIANGLES, meshAsset->getMeshSize(), GL_UNSIGNED_INT, nullptr);
            meshAsset->unbind();
        }
    }
};

}  // namespace shkyera
