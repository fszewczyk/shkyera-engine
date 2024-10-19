#pragma once

#include <string>
#include <iostream>

#include <AssetManager/Mesh.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class MeshComponent : public BaseComponent<MeshComponent> {
public:
    MeshComponent() = default;

    void setMesh(std::shared_ptr<Mesh> mesh) {
        _mesh = mesh;
    }

    Mesh const* getMesh() const {
        return _mesh.get();
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
};

} // namespace shkyera
