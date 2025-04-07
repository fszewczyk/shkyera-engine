#pragma once

#include <iostream>
#include <string>

#include <AssetManager/Material.hpp>
#include <AssetManager/Mesh.hpp>
#include <Components/BaseComponent.hpp>

namespace shkyera {

class OverlayModelComponent : public BaseComponent<OverlayModelComponent> {
 public:
  OverlayModelComponent() = default;

  void setMesh(AssetRef<Mesh> mesh) { _mesh = mesh; }

  void setMaterial(std::shared_ptr<Material> material) { _material = material; }

  Mesh const* getMesh() const { return _mesh.get(); }

  Material const* getMaterial() const { return _material.get(); }

  void updateImpl() const {
    if (_mesh) {
      _mesh->bind();
      glDrawElements(GL_TRIANGLES, _mesh->getMeshSize(), GL_UNSIGNED_INT, nullptr);
      _mesh->unbind();
    }
  }

 private:
  AssetRef<Mesh> _mesh;
  std::shared_ptr<Material> _material;
};

}  // namespace shkyera
