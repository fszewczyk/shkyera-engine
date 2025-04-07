#pragma once

#include <AssetManager/Asset.hpp>
#include <Components/AssetComponents/AssetLoader.hpp>

namespace shkyera {

template <typename AssetType>
struct AssetComponent {
  std::weak_ptr<AssetType> assetPtr{};
  std::unique_ptr<AssetLoader<AssetType>> constructionFunction;

  AssetComponent(std::unique_ptr<AssetLoader<AssetType>> constructor) : constructionFunction(std::move(constructor)) {}
};

}  // namespace shkyera
