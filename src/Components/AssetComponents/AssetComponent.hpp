#pragma once

#include <filesystem>

#include <AssetManager/Shader.hpp>

namespace shkyera {

template<typename AssetType>
struct AssetComponent {
    AssetComponent(std::function<AssetType()> constructor) : constructionFunction(constructor) {}

    std::weak_ptr<AssetType> shaderPtr{};
    std::function<AssetType()> constructionFunction;
};

} // namespace shkyera
