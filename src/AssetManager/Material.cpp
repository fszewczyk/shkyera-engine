#include <AssetManager/Material.hpp>

namespace shkyera {

Material::Material() 
    : diffuseColor(1.0f, 1.0f, 1.0f), specularColor(1.0f, 1.0f, 1.0f), shininess(32.0f) {}

Material::Material(const glm::vec3& diffuse, const glm::vec3& specular, float shininess) 
    : diffuseColor(diffuse), specularColor(specular), shininess(shininess) {}

const glm::vec3& Material::getDiffuseColor() const {
    return diffuseColor;
}

glm::vec3& Material::getDiffuseColor() {
    return diffuseColor;
}

const glm::vec3& Material::getSpecularColor() const {
    return specularColor;
}

glm::vec3& Material::getSpecularColor() {
    return specularColor;
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

void Material::setSpecularColor(const glm::vec3& specular) {
    specularColor = specular;
}

void Material::setShininess(float shininessValue) {
    shininess = shininessValue;
}

}
