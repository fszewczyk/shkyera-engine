#pragma once

#include <AssetManager/Material.hpp>
#include <AssetManager/Mesh.hpp>
#include <Components/ModelComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/Common/AssetSelector.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class ModelComponentUI : public ComponentUI {
   public:
    ModelComponentUI(std::shared_ptr<Registry> registry, ModelComponent* modelComponent);

    void draw() override;
    void setOnMeshUpdate(std::function<void()> callback);

   private:
    ModelComponent* _modelComponent;
    std::shared_ptr<Registry> _registry;

    AssetSelector<Mesh> _meshSelector;
    AssetSelector<Material> _materialSelector;
    std::function<void()> _onMeshUpdate;
};

}  // namespace shkyera
