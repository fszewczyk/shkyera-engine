#pragma once

#include <AssetManager/Material.hpp>
#include <Components/BillboardComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/Common/AssetSelector.hpp>
#include <UI/Common/EnumSelector.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class BillboardComponentUI : public ComponentUI {
 public:
  BillboardComponentUI(std::shared_ptr<Registry> registry, BillboardComponent<>* billboardComponent);

  void draw() override;

 private:
  BillboardComponent<>* _billboardComponent;
  std::shared_ptr<Registry> _registry;

  AssetSelector<Material> _materialSelector;
  EnumSelector<BillboardComponent<>::Orientation> _orientationSelector;
  EnumSelector<BillboardComponent<>::Scale> _scaleSelector;
  EnumSelector<BillboardComponent<>::Occlusion> _occlusionSelector;
};

}  // namespace shkyera
