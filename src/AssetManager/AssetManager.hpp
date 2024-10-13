#pragma once

#include <unordered_map>
#include <filesystem>

#include <AssetManager/Asset.hpp>

namespace shkyera {

class AssetManager {
    public:
        static AssetManager& getInstance();

        template<typename AssetType, typename... Args>
        std::shared_ptr<AssetType> getAsset(const std::filesystem::path& path, Args... args) {
            if (_assets.contains(path.string())) {
                if (auto asset = _assets.at(path.string())) {
                    return std::static_pointer_cast<AssetType>(asset);
                }
            }

            auto asset = std::make_shared<AssetType>(path, args...);
            _assets[path.string()] = asset;
            return asset;
        }

    private:
        AssetManager() = default;

        std::unordered_map<std::string, std::shared_ptr<Asset>> _assets;
};

}
