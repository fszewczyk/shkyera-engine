#pragma once

#include <AssetManager/Asset.hpp>
#include <AssetManager/Texture.hpp>
#include <Components/AssetComponents/AssetComponent.hpp>
#include <UI/Common/AssetSelector.hpp>
#include <UI/Common/BooleanSelector.hpp>
#include <UI/Common/ColorSelector.hpp>
#include <UI/Common/FloatSlider.hpp>
#include <UI/Widget.hpp>

namespace shkyera {

class InspectorWidget : public Widget {
 public:
  using Widget::Widget;

  InspectorWidget(std::string name, std::shared_ptr<Registry> registry);

  /**
     * @brief Implementation of the abstract `draw` method to render the profiler widget.
     */
  virtual void draw() override;

 private:
  bool updateAssetSelection();

  template <typename AssetType, typename InspectorType>
  bool tryResetInspector() {
    if (!_selectedAsset) {
      _assetInspector.reset();
      return false;
    }

    if (_registry->hasComponent<AssetComponent<AssetType>>(*_selectedAsset)) {
      _assetInspector = std::make_unique<InspectorType>(_registry.get(), *_selectedAsset);
      return true;
    }

    return false;
  }

  class AssetInspector {
   public:
    virtual ~AssetInspector() = default;
    virtual void draw() = 0;
  };

  class MaterialInspector : public AssetInspector {
   public:
    MaterialInspector(Registry* registry, AssetHandle assetHandle);
    void draw();

   private:
    ColorSelector _albedoColorSelector;
    ColorSelector _emissiveColorSelector;
    BooleanSelector _litMaterialCheckbox;
    FloatSlider _roughnessSlider;
    FloatSlider _metallicSlider;
    FloatSlider _emissiveStrengthSlider;
    FloatSlider _normalMapStrengthSlider;
    TextureAssetSelector _albedoTextureSelector;
    TextureAssetSelector _normalTextureSelector;
    TextureAssetSelector _roughnessTextureSelector;
    TextureAssetSelector _metallicTextureSelector;
    TextureAssetSelector _emissiveTextureSelector;
  };

  class TextureInspector : public AssetInspector {
   public:
    TextureInspector(Registry* registry, AssetHandle assetHandle);

    void draw();

   private:
    AssetRef<Texture> _texture;
  };

  std::shared_ptr<Registry> _registry;
  std::optional<AssetHandle> _selectedAsset;
  std::unique_ptr<AssetInspector> _assetInspector;
};

}  // namespace shkyera
