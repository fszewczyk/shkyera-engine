#pragma once

#include <glm/glm.hpp>
#include <AssetManager/Asset.hpp>

namespace shkyera {

class Material : public Asset {
public:
    // Constructors
    Material();
    Material(const glm::vec3& diffuse, float shininess);

    const glm::vec3& getDiffuseColor() const;
    glm::vec3& getDiffuseColor();

    float getShininess() const;
    float& getShininess();

    void setDiffuseColor(const glm::vec3& diffuse);
    void setShininess(float shininessValue);

private:
    glm::vec3 diffuseColor;
    float shininess;
};

}
