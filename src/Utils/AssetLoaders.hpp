#pragma once

#include <filesystem>

#include <AssetManager/Asset.hpp>
#include <AssetManager/CubeMap.hpp>
#include <AssetManager/Material.hpp>
#include <AssetManager/Shader.hpp>
#include <Common/Assert.hpp>
#include <Common/Types.hpp>
#include <Components/AssetComponents/AssetComponent.hpp>
#include <Components/AssetComponents/AssetLoader.hpp>
#include <Components/AssetComponents/DirectoryComponent.hpp>
#include <ECS/Registry.hpp>
#include <Utils/AssetUtils.hpp>

namespace shkyera::utils::assets {

class ShaderLoader : public AssetLoader<Shader> {
 public:
  ShaderLoader(std::filesystem::path path, Shader::Type type) : path(std::move(path)), type(type) {}

  Shader operator()() override { return Shader(path, type); }

  std::filesystem::path path;
  Shader::Type type;
};

class MaterialLoader : public AssetLoader<Material> {
 public:
  explicit MaterialLoader(Registry* registry, Material mat_) : _registry(registry), mat(std::move(mat_)) {
    /// Making sure that the loader does not extend the lifetime of the underlying assets
    std::get<AssetRef<Texture>>(mat.albedoTexture).reset();
    std::get<AssetRef<Texture>>(mat.normalTexture).reset();
    std::get<AssetRef<Texture>>(mat.roughnessTexture).reset();
    std::get<AssetRef<Texture>>(mat.metallicTexture).reset();
    std::get<AssetRef<Texture>>(mat.emissiveTexture).reset();
  }

  Material operator()() override {
    /// Copying the Material is required so that the mat member does not hold a reference
    /// to the texture

    auto matCopy = mat;
    constructTextureMember(matCopy, &Material::albedoTexture);
    constructTextureMember(matCopy, &Material::normalTexture);
    constructTextureMember(matCopy, &Material::roughnessTexture);
    constructTextureMember(matCopy, &Material::metallicTexture);
    constructTextureMember(matCopy, &Material::emissiveTexture);
    return matCopy;
  }

  Material mat;

 private:
  void constructTextureMember(Material& mat, auto textureMember) {
    if (const auto& assetHandleOpt = std::get<OptionalAssetHandle>(mat.*textureMember)) {
      if (_registry->hasComponent<AssetComponent<Texture>>(*assetHandleOpt)) {
        auto& textureAssetComponent = _registry->getComponent<AssetComponent<Texture>>(*assetHandleOpt);
        std::get<AssetRef<Texture>>(mat.*textureMember) = read(textureAssetComponent);
      }
    }
  }

  Registry* _registry;
};

class CubeMapLoader : public AssetLoader<CubeMap> {
 public:
  using Faces = std::array<HandleAndAsset<Image>, 6>;

  CubeMapLoader(Registry* registry, Faces faces_) : _registry(registry), faces(std::move(faces_)) {
    resetInternalReferences();
  }

  CubeMap operator()() override {
    CubeMap out;
    out.loadFaces(_registry, faces[0], faces[1], faces[2], faces[3], faces[4], faces[5]);
    return out;
  }

  Faces faces;

 private:
  void resetInternalReferences() {
    for (auto& f : faces) {
      std::get<AssetRef<Image>>(f).reset();
    }
  }

  Registry* _registry;
};

}  // namespace shkyera::utils::assets
