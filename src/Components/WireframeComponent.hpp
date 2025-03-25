#pragma once

#include <AssetManager/Asset.hpp>
#include <AssetManager/Wireframe.hpp>
#include <Common/Serialization.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

struct WireframeComponent : public BaseComponent<WireframeComponent> {
    HandleAndAsset<Wireframe> wireframe{};

    template <typename Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(wireframe));
    }

    void updateImpl() const {
        const auto& wireframeAsset = std::get<AssetRef<Wireframe>>(wireframe);
        if (wireframeAsset) {
            wireframeAsset->bind();
            glDrawArrays(GL_LINES, 0, wireframeAsset->getEdgeCount());
            wireframeAsset->unbind();
        }
    }
};

}  // namespace shkyera
