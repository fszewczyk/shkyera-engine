#pragma once

#include <iostream>
#include <string>

#include <AssetManager/Material.hpp>
#include <AssetManager/Mesh.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class ModelComponent : public BaseComponent<ModelComponent> {
   public:
    HandleAndAsset<Mesh> mesh{};
    HandleAndAsset<Material> material{};

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
