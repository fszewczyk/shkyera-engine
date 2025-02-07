#pragma once

#include <filesystem>

namespace shkyera {

template<typename AssetType>
class PathConstructibleAsset {
    public:
        PathConstructibleAsset() = default;
        
        PathConstructibleAsset(std::optional<std::filesystem::path> path)
        {
            if(path)
            {
                assetToPath[static_cast<AssetType const*>(this)] = *path;
            }
        }

        PathConstructibleAsset(PathConstructibleAsset&& other) noexcept
        {
            auto it = assetToPath.find(static_cast<AssetType const*>(&other));
            if (it != assetToPath.end()) {
                assetToPath[static_cast<AssetType const*>(this)] = std::move(it->second);
                assetToPath.erase(it);
            }
        }

        PathConstructibleAsset& operator=(PathConstructibleAsset&& other) noexcept
        {
            if (this != &other) 
            {
                assetToPath.erase(static_cast<AssetType const*>(this));

                auto it = assetToPath.find(static_cast<AssetType const*>(&other));
                if (it != assetToPath.end()) 
                {
                    assetToPath[static_cast<AssetType const*>(this)] = std::move(it->second);
                    assetToPath.erase(it);
                }
            }
            return *this;
        }

        virtual ~PathConstructibleAsset()
        {
            assetToPath.erase(static_cast<AssetType const*>(this));
        }

        std::optional<std::filesystem::path> getPath() const
        {
            const auto pathIt = assetToPath.find(static_cast<AssetType const*>(this));
            if(pathIt == assetToPath.end())
            {
                return std::nullopt;
            }
            return pathIt->second;
        }

    private:
        inline static std::unordered_map<AssetType const*, std::filesystem::path> assetToPath{};
};

}
