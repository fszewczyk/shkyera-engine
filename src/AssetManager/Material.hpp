#pragma once

#include <AssetManager/Asset.hpp>
#include <AssetManager/Texture.hpp>
#include <Common/InstanceCounter.hpp>
#include <glm/glm.hpp>

namespace shkyera {

struct Material : InstanceCounter<Material> {
  Material() = default;

  bool lit{true};
  float alphaMultiplier{1.0f};
  glm::vec3 albedo{1.0f, 1.0f, 1.0f};
  glm::vec3 emissive{1.0f, 1.0f, 1.0f};
  float emissiveStrength{0.0f};
  float normalMapStrength{1.0f};
  float roughness{1.0f};
  float metallic{0.0f};
  HandleAndAsset<Texture> albedoTexture;
  HandleAndAsset<Texture> normalTexture;
  HandleAndAsset<Texture> roughnessTexture;
  HandleAndAsset<Texture> metallicTexture;
  HandleAndAsset<Texture> emissiveTexture;
};

}  // namespace shkyera
