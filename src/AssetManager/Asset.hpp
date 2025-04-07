#pragma once

#include <ECS/Registry.hpp>
#include <filesystem>

namespace shkyera {

using AssetHandle = Entity;
using OptionalAssetHandle = std::optional<AssetHandle>;

template <typename AssetType>
using AssetRef = std::shared_ptr<AssetType>;

template <typename AssetType>
using HandleAndAsset = std::pair<OptionalAssetHandle, AssetRef<AssetType>>;

}  // namespace shkyera
