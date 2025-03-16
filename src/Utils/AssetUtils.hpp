#pragma once

#include <filesystem>

#include <AssetManager/Asset.hpp>
#include <Common/Types.hpp>
#include <Components/AssetComponents/AssetComponent.hpp>
#include <Components/AssetComponents/DirectoryComponent.hpp>
#include <ECS/Registry.hpp>

namespace shkyera::utils::assets {

/**
 * Registers the asset at the specified path in the registry. Asset type will be deduced.
 * It can be either a file or a directory. This function does NOT traverse the filesystem
 * if the asset is a directory.
 * 
 * @param root Path to the asset (directory or file) to register
 * @param registry Non-owning pointer to the ECS registry
 * @return If successful, the handle of the registerd asset
 */
std::optional<AssetHandle> registerSingle(std::filesystem::path path, Registry* registry);

/**
 * If path points to a directory, it recursively traverses the root directory and registers all assets, including both
 * files and directories. If path poitns to a file, it registers a single file.
 * 
 * @param root Path to the initial asset
 * @param registry Non-owning pointer to the ECS registry
 * @return If successful, the handle of the asset at the provided path
 */
std::optional<AssetHandle> registerAll(std::filesystem::path path, Registry* registry);

/**
 * Retrieves all subdirectories from the specified directory asset.
 * 
 * @param directory Handle to the directory asset whose subdirectories are to be retrieved
 * @param registry Pointer to the ECS registry to access entity components
 * @return A vector of AssetHandles representing the subdirectories of the provided directory asset
 */
std::vector<AssetHandle> getSubdirectories(AssetHandle directory, Registry const* registry);

/**
 * Gets the name of the directory from the provided DirectoryComponent.
 * 
 * @param directoryComponent DirectoryComponent that holds the directory's name
 * @return The name of the directory
 */
std::string getName(const DirectoryComponent& directoryComponent);

/**
 * Reads the asset from the provided AssetComponent by either retrieving the cached asset or constructing
 * a new asset and storing it in the component.
 * 
 * @tparam AssetType The type of the asset to read
 * @param assetComponent Reference to the AssetComponent holding the asset data
 * @return A shared pointer to the asset of type AssetType
 */
template <typename AssetType>
AssetRef<AssetType> read(AssetComponent<AssetType>& assetComponent) {
    if (auto asset = assetComponent.assetPtr.lock()) {
        return asset;
    }

    auto asset = std::make_shared<AssetType>(assetComponent.constructionFunction());
    assetComponent.assetPtr = asset;
    return asset;
}

/**
 * Constructs an asset of the specified type from a specified path with given constructor arguments.
 * 
 * @tparam AssetType The type of the asset to read
 * @tparam Args Constructor argument pack
 * @param assetComponent Reference to the AssetComponent holding the asset data
 * @param args Constructor parameters
 * @return A shared pointer to the asset of type AssetType
 */
template <PathConstructible AssetType, typename... Args>
AssetRef<AssetType> read(const std::filesystem::path& path, Args&&... args) {
    return std::make_shared<AssetType>(path, std::forward<Args>(args)...);
}

/**
 * Adds a new asset of the specified type to the registry and returns the asset handle
 * along with a reference to the associated AssetComponent.
 * 
 * @tparam AssetType The type of the asset to add
 * @tparam Args The argument types required for constructing the asset
 * @param registry Pointer to the ECS registry
 * @param args Arguments to be forwarded to the constructor of AssetComponent
 * @return A pair containing the handle of the new asset and a reference to its AssetComponent
 */
template <typename AssetType, typename... Args>
std::pair<AssetHandle, AssetComponent<AssetType>&> add(Registry* registry, Args&&... args) {
    auto assetHandle = registry->addEntity();
    auto& assetComponent = registry->addComponent<AssetComponent<AssetType>>(assetHandle, std::forward<Args>(args)...);
    return {assetHandle, assetComponent};
}

/**
 * Adds a new asset to the ECS registry, reads it, and returns a shared pointer to the asset.
 * This is a combination of adding the asset and reading it from the AssetComponent in one step.
 * 
 * @tparam AssetType The type of the asset to add and read
 * @tparam Args The argument types required for constructing the asset
 * @param registry Pointer to the ECS registry
 * @param args Arguments to be forwarded to the constructor of AssetComponent
 * @return A shared pointer to the asset of type AssetType
 */
template <typename AssetType, typename... Args>
AssetRef<AssetType> addAndRead(Registry* registry, Args&&... args) {
    auto&& [_entity, component] = add<AssetType>(registry, std::forward<Args>(args)...);
    return read(component);
}

/**
 * At first call, adds a new asset to the ECS registry, reads it and returns a shared pointer to the asset.
 * At later calls, it returns an already loaded asset. This function GUARANTEES that a once loaded asset
 * will never be unloaded. It should not be used for loading large assets. The permanent assets DO NOT have a handle
 * and DO NOT exist in any registry;
 * @tparam AssetType PathConstructible AssetType defining the type of the asset to add and read
 * @tparam Args The argument types required for constructing the asset
 * @param path Path to the asset
 * @param args Arguments to be forwarded to the constructor of AssetComponent
 * @return A shared pointer to the asset of type AssetType
 */
template <typename AssetType, typename... Args>
AssetRef<AssetType> readPermanent(const std::filesystem::path& path, Args&&... args) {
    namespace fs = std::filesystem;
    static std::unordered_map<fs::path, AssetRef<AssetType>, GlobalPathHash, GlobalPathEqual> permanentAssets;

    if (!permanentAssets.contains(path)) {
        const auto assetPtr = std::make_shared<AssetType>(path, args...);
        permanentAssets[path] = assetPtr;
        return assetPtr;
    }

    return permanentAssets.at(path);
}

/**
 * At first call, construct an Asset using the provided factory method.
 * At later calls, return the oprevously constructed Asset.
 * @param registry Pointer to the ECS registry
 * @return A shared pointer to the asset of type AssetType
 */
template <typename AssetType, AssetType (*FactoryMethod)(), typename... Args>
AssetRef<AssetType> fromFactory(Registry* registry, Args&&... args) {
    static auto mesh = addAndRead<AssetType>(registry, []() { return FactoryMethod(); }, std::forward<Args>(args)...);
    return mesh;
}

}  // namespace shkyera::utils::assets
