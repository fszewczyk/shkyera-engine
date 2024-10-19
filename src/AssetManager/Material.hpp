#pragma once

#include <glm/glm.hpp>
#include <AssetManager/Asset.hpp>

namespace shkyera {

class Material : public Asset {
public:
    // Constructors
    Material();
    Material(const glm::vec3& diffuse, const glm::vec3& specular, float shininess);

    const glm::vec3& getDiffuseColor() const;
    glm::vec3& getDiffuseColor();

    const glm::vec3& getSpecularColor() const;
    glm::vec3& getSpecularColor();

    float getShininess() const;
    float& getShininess();

    void setDiffuseColor(const glm::vec3& diffuse);
    void setSpecularColor(const glm::vec3& specular);
    void setShininess(float shininessValue);

private:
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
};

}
