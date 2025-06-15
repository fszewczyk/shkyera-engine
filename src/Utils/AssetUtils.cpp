#include <filesystem>
#include <memory>

#include <AssetManager/Mesh.hpp>
#include <AssetManager/Shader.hpp>
#include <AssetManager/Audio.hpp>
#include <AssetManager/Texture.hpp>
#include <Common/Logger.hpp>
#include <Components/AssetComponents/AssetRoot.hpp>
#include <Components/AssetComponents/DirectoryComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Utils/AssetLoaders.hpp>
#include <Utils/AssetUtils.hpp>

namespace shkyera::utils::assets {

template <typename... Args>
static bool oneOf(const std::string& target, Args&&... args) {
  return (... || (target == args));
}

std::optional<AssetHandle> registerSingle(std::filesystem::path path, Registry* registry) {
  const auto handle = registry->addEntity();
  auto& nameComponent = registry->addComponent<NameComponent>(handle);
  if (std::filesystem::is_directory(path)) {
    nameComponent.setName(path.stem());
    registry->addComponent<DirectoryComponent>(handle, path);
  } else {
    nameComponent.setName(path.filename());
    const std::string fileExtension = path.extension();
    if (oneOf(fileExtension, ".jpg", ".jpeg", ".png")) {
      registry->addComponent<AssetComponent<Texture>>(handle, std::make_unique<PathAssetLoader<Texture>>(path));
    } else if (oneOf(fileExtension, ".obj")) {
      registry->addComponent<AssetComponent<Mesh>>(handle, std::make_unique<PathAssetLoader<Mesh>>(path));
    } else if (oneOf(fileExtension, ".glsl")) {
      registry->addComponent<AssetComponent<Shader>>(handle,
                                                     std::make_unique<ShaderLoader>(path, Shader::Type::Fragment));
    } else if (oneOf(fileExtension, ".mp3", ".wav", ".ogg")) {
      registry->addComponent<AssetComponent<Audio>>(handle, std::make_unique<PathAssetLoader<Audio>>(path));
    } else {
      registry->removeEntity(handle);
      Logger::VERBOSE(std::string("Unrecognized asset type: ") + path.c_str());
      return std::nullopt;
    }
  }
  return handle;
}

std::optional<AssetHandle> registerAll(std::filesystem::path path, Registry* registry) {
  if (!std::filesystem::is_directory(path)) {
    return registerSingle(path, registry);
  }

  if (const auto rootHandleOpt = registerSingle(path, registry)) {
    for (const auto& subPath : std::filesystem::directory_iterator(path)) {
      if (const auto childHandleOpt = registerAll(subPath, registry)) {
        registry->getHierarchy().attributeChild(*rootHandleOpt, *childHandleOpt);
      }
    }

    return rootHandleOpt;
  }

  return std::nullopt;
}

std::vector<AssetHandle> getSubdirectories(AssetHandle directory, Registry const* registry) {
  const auto& directoryContents = registry->getHierarchy().getChildren(directory);
  std::vector<AssetHandle> subDirectories;
  std::ranges::copy_if(directoryContents, std::back_inserter(subDirectories),
                       [registry](AssetHandle asset) { return registry->hasComponent<DirectoryComponent>(asset); });
  return subDirectories;
}

}  // namespace shkyera::utils::assets
