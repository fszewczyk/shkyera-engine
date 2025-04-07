#pragma once

#include <filesystem>

#include <AssetManager/Asset.hpp>
#include <Common/Assert.hpp>
#include <Common/Types.hpp>
#include <Components/AssetComponents/AssetComponent.hpp>
#include <Components/AssetComponents/AssetLoader.hpp>
#include <Components/AssetComponents/DirectoryComponent.hpp>
#include <ECS/Registry.hpp>
#include <Serialization/Builders.hpp>

namespace shkyera::utils::assets {

template <typename AssetType>
class PathAssetLoader : public AssetLoader<AssetType> {
 public:
  PathAssetLoader(std::filesystem::path path_) : path(std::move(path_)) {}

  ~PathAssetLoader() = default;

  AssetType operator()() override { return AssetType(path); }

  std::filesystem::path path;
};

template <typename AssetType>
class FactoryAssetLoader : public AssetLoader<AssetType> {
 public:
  using Factory = typename AssetType::Factory;

  FactoryAssetLoader(Factory::Type type_) : type(type_) {}

  ~FactoryAssetLoader() = default;

  AssetType operator()() override { return Factory::create(type); }

  Factory::Type type;
};

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

  auto asset = std::make_shared<AssetType>((*assetComponent.constructionFunction)());
  assetComponent.assetPtr = asset;
  return asset;
}

template <typename AssetType>
AssetRef<AssetType> read(Registry* registry, HandleAndAsset<AssetType>& handleAndAsset) {
  if (const auto assetRef = std::get<AssetRef<AssetType>>(handleAndAsset)) {
    return assetRef;
  }

  if (const auto assetHandleOpt = std::get<OptionalAssetHandle>(handleAndAsset)) {
    SHKYERA_ASSERT(registry->hasComponent<AssetComponent<AssetType>>(*assetHandleOpt),
                   "{} does not have an associated Asset Component for {}", *assetHandleOpt, typeid(AssetType).name());
    return read<AssetType>(registry->getComponent<AssetComponent<AssetType>>(*assetHandleOpt));
  }

  return nullptr;
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
template <PathConstructible AssetType>
AssetRef<AssetType> read(const std::filesystem::path& path) {
  return std::make_shared<AssetType>(path);
}

template <typename AssetType>
AssetRef<AssetType> read(typename AssetType::Factory::Type type) {
  auto component = std::make_shared<AssetType>(AssetType::Factory::create(type));
  return component;
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
template <typename AssetType>
HandleAndAsset<AssetType> add(Registry* registry, std::unique_ptr<AssetLoader<AssetType>> loader) {
  auto assetHandle = registry->addEntity();
  auto& assetComponent = registry->addComponent<AssetComponent<AssetType>>(assetHandle, std::move(loader));
  return {assetHandle, read(assetComponent)};
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
template <typename AssetType>
HandleAndAsset<AssetType> add(Registry* registry, std::filesystem::path&& path) {
  return add<AssetType>(registry, std::make_unique<PathAssetLoader<AssetType>>(path));
}

/**
 * At first call, construct an Asset using the provided factory method.
 * At later calls, return the oprevously constructed Asset.
 * @param registry Pointer to the ECS registry
 * @return A shared pointer to the asset of type AssetType
 */
template <typename AssetType>
HandleAndAsset<AssetType> add(Registry* registry, typename AssetType::Factory::Type type) {
  return add<AssetType>(registry, std::make_unique<FactoryAssetLoader<AssetType>>(type));
}

/**
 * At first call, adds a new asset to the ECS registry, reads it and returns a shared pointer to the asset.
 * At later calls, it returns an already loaded asset. This function GUARANTEES that a once loaded asset
 * will never be unloaded. It should not be used for loading large assets. The permanent assets DO NOT have a handle
 * and DO NOT exist in any registry;
 *
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

template <typename AssetType>
AssetRef<AssetType> readPermanent(typename AssetType::Factory::Type type) {
  static std::unordered_map<int, AssetRef<AssetType>> permanentAssets;

  const auto typeInt = static_cast<int>(type);
  if (!permanentAssets.contains(typeInt)) {
    const auto assetPtr = std::make_shared<AssetType>(AssetType::Factory::create(type));
    permanentAssets[typeInt] = assetPtr;
    return assetPtr;
  }

  return permanentAssets.at(typeInt);
}

}  // namespace shkyera::utils::assets
