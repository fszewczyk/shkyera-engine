#pragma once

#include <string>
#include <iostream>

#include <AssetManager/Mesh.hpp>
#include <AssetManager/Material.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class ModelComponent : public BaseComponent<ModelComponent> {
public:
    ModelComponent() = default;

    void setMesh(std::shared_ptr<Mesh> mesh) {
        _mesh = mesh;
    }

    void setMaterial(std::shared_ptr<Material> material) {
        _material = material;
    }

    Mesh const* getMesh() const {
        return _mesh.get();
    }

    Material const* getMaterial() const {
        return _material.get();
    }

    void updateImpl() const {
        if(_mesh) {
            _mesh->bind();
            glDrawElements(GL_TRIANGLES, _mesh->getMeshSize(), GL_UNSIGNED_INT, nullptr);
            _mesh->unbind();
        }
    }

private:
    std::shared_ptr<Mesh> _mesh;
    std::shared_ptr<Material> _material;
};

} // namespace shkyera
