#pragma once

#include <Common/Serialization.hpp>
#include <ECS/Registry.hpp>
#include <filesystem>

namespace shkyera {

using AssetHandle = Entity;
using OptionalAssetHandle = std::optional<AssetHandle>;

template <typename AssetType>
using AssetRef = std::shared_ptr<AssetType>;

template <typename AssetType>
using HandleAndAsset = std::pair<OptionalAssetHandle, AssetRef<AssetType>>;

template <typename Archive, typename AssetType>
void serialize(Archive& archive, HandleAndAsset<AssetType>& asset) {
    const auto assetHandle = std::get<OptionalAssetHandle>(asset);
    archive(CEREAL_NVP(assetHandle));
}

}  // namespace shkyera
