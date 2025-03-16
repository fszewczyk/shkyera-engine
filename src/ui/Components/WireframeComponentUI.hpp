#pragma once

#include <Components/WireframeComponent.hpp>
#include <UI/Common/AssetSelector.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class WireframeComponentUI : public ComponentUI {
   public:
    WireframeComponentUI(std::shared_ptr<Registry> registry, WireframeComponent* wireframeComponent);

    void draw() override;

   private:
    std::shared_ptr<Registry> _registry;
    WireframeComponent* _wireframeComponent;
    AssetSelector<Wireframe> _wireframeSelector;
};

}  // namespace shkyera
