#include <AssetManager/Material.hpp>

namespace shkyera {

Material::Material(const glm::vec3& albedo, float roughness, float metallic)
    : albedo(albedo),
      roughness(roughness),
      metallic(metallic) {}

}  // namespace shkyera