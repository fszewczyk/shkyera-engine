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
                if (auto asset = _assets.at(path.string()).lock()) {
                    return std::static_pointer_cast<AssetType>(asset);
                }
            }

            auto asset = std::make_shared<AssetType>(path, args...);
            _assetFilePaths[asset.get()] = path;
            _assets[path.string()] = asset;
            return asset;
        }

        template<typename AssetType>
        std::string getFilePath(AssetType* asset) {
            void* ptr = (void *)(asset);
            if(_assetFilePaths.find(ptr) == _assetFilePaths.end()) {
                return nullptr;
            }
            return _assetFilePaths.at(ptr);
        }

    private:
        AssetManager() = default;

        std::unordered_map<std::string, std::weak_ptr<Asset>> _assets;
        std::unordered_map<void*, std::string> _assetFilePaths;
};

}
