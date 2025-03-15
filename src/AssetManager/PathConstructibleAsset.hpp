#pragma once

#include <filesystem>

#include <Common/InstanceCounter.hpp>

namespace shkyera {

template<typename AssetType>
    class PathConstructibleAsset : InstanceCounter<AssetType> {
    public:
        PathConstructibleAsset() = default;
        
        PathConstructibleAsset(std::optional<std::filesystem::path> path)
        {
            if(path)
            {
                assetToPath[static_cast<AssetType const*>(this)] = *path;
            }
        }

        PathConstructibleAsset(PathConstructibleAsset&& other) noexcept : InstanceCounter<AssetType>(std::move(other))
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
                InstanceCounter<AssetType>::operator=(std::move(other));

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
