#include <AssetManager/Material.hpp>

namespace shkyera {

Material::Material() 
    : diffuseColor(1.0f, 1.0f, 1.0f), shininess(32.0f) {}

Material::Material(const glm::vec3& diffuse, float shininess) 
    : diffuseColor(diffuse), shininess(shininess) {}

const glm::vec3& Material::getDiffuseColor() const {
    return diffuseColor;
}

glm::vec3& Material::getDiffuseColor() {
    return diffuseColor;
}

float Material::getShininess() const {
    return shininess;
}

float& Material::getShininess() {
    return shininess;
}


void Material::setDiffuseColor(const glm::vec3& diffuse) {
    diffuseColor = diffuse;
}

void Material::setShininess(float shininessValue) {
    shininess = shininessValue;
}

}
