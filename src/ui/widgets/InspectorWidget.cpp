#include <type_traits>

#include <imgui_internal.h>
#include "imgui.h"

#include <AssetManager/Material.hpp>
#include <Components/AssetComponents/SelectedAssetComponent.hpp>
#include <Components/NameComponent.hpp>
#include <ECS/Registry.hpp>
#include <UI/Common/Style.hpp>
#include <UI/Common/Utils.hpp>
#include <UI/Widgets/InspectorWidget.hpp>
#include <Utils/AssetLoaders.hpp>
#include <Utils/AssetUtils.hpp>

namespace shkyera {

InspectorWidget::InspectorWidget(std::string name, std::shared_ptr<Registry> registry)
    : Widget(std::move(name)), _registry(std::move(registry)) {}

void InspectorWidget::draw() {
  const auto newAssetSelected = updateAssetSelection();
  if (newAssetSelected) {
    const bool success =
        tryResetInspector<Material, MaterialInspector>() || tryResetInspector<Texture, TextureInspector>();
  }

  ImGui::Begin(_name.c_str());
  if (_selectedAsset && _registry->hasComponent<NameComponent>(*_selectedAsset)) {
    ImGui::PushFont(style::HUGE_FONT);
    ImGui::TextUnformatted(_registry->getComponent<NameComponent>(*_selectedAsset).getName().c_str());
    ImGui::PopFont();

    if (_assetInspector) {
      _assetInspector->draw();
    }
  } else {
    ImGui::PushFont(style::HUGE_FONT);
    ImGui::TextUnformatted("No Asset Selected");
    ImGui::PopFont();
  }

  ImGui::End();
}

bool InspectorWidget::updateAssetSelection() {
  const auto& selectedAssetSet = _registry->getComponentSet<SelectedAssetComponent>();
  if (selectedAssetSet.begin() == selectedAssetSet.end()) {
    _selectedAsset.reset();
    return false;
  }
  const auto previouslySelected = _selectedAsset;
  _selectedAsset = std::get<AssetHandle>(*selectedAssetSet.begin());
  return (!previouslySelected) || (*previouslySelected != *_selectedAsset);
}

InspectorWidget::MaterialInspector::MaterialInspector(Registry* registry, AssetHandle handle)
    : _albedoColorSelector("Color"),
      _emissiveColorSelector("Color"),
      _roughnessSlider("Roughness", 0.05f, 1.0f),
      _litMaterialCheckbox("Lit", false),
      _metallicSlider("Metallic", 0.05f, 1.0f),
      _emissiveStrengthSlider("Strength", 0.0f, 50.0f),
      _normalMapStrengthSlider("Multiplier", 0.0f, 2.0f) {
  if (registry->hasComponent<AssetComponent<Material>>(handle)) {
    auto& materialAsset = registry->getComponent<AssetComponent<Material>>(handle);
    const auto materialRef = utils::assets::read(materialAsset);
    _albedoColorSelector = ColorSelector("Albedo Color", materialRef->albedo);
    _emissiveColorSelector = ColorSelector("Emissive Color", materialRef->emissive);
    _litMaterialCheckbox = BooleanSelector("Lit", materialRef->lit);
    _roughnessSlider = FloatSlider("Roughness", materialRef->roughness, 0.1f, 1.0f);
    _metallicSlider = FloatSlider("Metallic", materialRef->metallic, 0.1f, 1.0f);
    _emissiveStrengthSlider = FloatSlider("Strength", materialRef->emissiveStrength, 0.0f, 50.0f);
    _normalMapStrengthSlider = FloatSlider("Multiplier", materialRef->normalMapStrength, 0.0f, 2.0f);
    _albedoTextureSelector =
        TextureAssetSelector("Albedo Texture", registry, std::get<OptionalAssetHandle>(materialRef->albedoTexture));
    _normalTextureSelector =
        TextureAssetSelector("Normal Map", registry, std::get<OptionalAssetHandle>(materialRef->normalTexture));
    _roughnessTextureSelector =
        TextureAssetSelector("Roughness Map", registry, std::get<OptionalAssetHandle>(materialRef->roughnessTexture));
    _metallicTextureSelector =
        TextureAssetSelector("Metallic Map", registry, std::get<OptionalAssetHandle>(materialRef->metallicTexture));
    _emissiveTextureSelector =
        TextureAssetSelector("Emissive Map", registry, std::get<OptionalAssetHandle>(materialRef->emissiveTexture));

    const auto updateConstructionFunction = [registry, handle, matCopy = *materialRef]() {
      if (registry->hasComponent<AssetComponent<Material>>(handle)) {
        auto& materialAsset = registry->getComponent<AssetComponent<Material>>(handle);
        materialAsset.constructionFunction =
            std::make_unique<utils::assets::MaterialLoader>(registry, std::move(matCopy));
      }
    };

    const auto setMaterialUpdateCallback = [&](auto& selector, auto materialProperty) {
      selector.setUpdateCallback([updateConstructionFunction, materialRef, materialProperty](const auto& value) {
        (*materialRef).*materialProperty = value;
        updateConstructionFunction();
      });
    };

    const auto setTextureClearCallback = [&](auto& selector, auto materialProperty) {
      selector.setClearCallback([updateConstructionFunction, materialRef, materialProperty]() {
        std::get<OptionalAssetHandle>((*materialRef).*materialProperty).reset();
        std::get<AssetRef<Texture>>((*materialRef).*materialProperty).reset();
        updateConstructionFunction();
      });
    };

    const auto setTextureUpdateCallback = [&](auto& selector, auto materialProperty) {
      selector.setUpdateCallback([registry, updateConstructionFunction, materialRef,
                                  materialProperty](const auto& textureHandle) {
        if (registry->hasComponent<AssetComponent<Texture>>(textureHandle)) {
          auto& textureAssetComponent = registry->getComponent<AssetComponent<Texture>>(textureHandle);
          std::get<OptionalAssetHandle>((*materialRef).*materialProperty) = textureHandle;
          std::get<AssetRef<Texture>>((*materialRef).*materialProperty) = utils::assets::read(textureAssetComponent);
          updateConstructionFunction();
        }
      });
    };

    setMaterialUpdateCallback(_albedoColorSelector, &Material::albedo);
    setMaterialUpdateCallback(_roughnessSlider, &Material::roughness);
    setMaterialUpdateCallback(_metallicSlider, &Material::metallic);
    setMaterialUpdateCallback(_emissiveColorSelector, &Material::emissive);
    setMaterialUpdateCallback(_emissiveStrengthSlider, &Material::emissiveStrength);
    setMaterialUpdateCallback(_normalMapStrengthSlider, &Material::normalMapStrength);

    setTextureClearCallback(_albedoTextureSelector, &Material::albedoTexture);
    setTextureClearCallback(_normalTextureSelector, &Material::normalTexture);
    setTextureClearCallback(_roughnessTextureSelector, &Material::roughnessTexture);
    setTextureClearCallback(_metallicTextureSelector, &Material::metallicTexture);
    setTextureClearCallback(_emissiveTextureSelector, &Material::emissiveTexture);

    setTextureUpdateCallback(_albedoTextureSelector, &Material::albedoTexture);
    setTextureUpdateCallback(_normalTextureSelector, &Material::normalTexture);
    setTextureUpdateCallback(_roughnessTextureSelector, &Material::roughnessTexture);
    setTextureUpdateCallback(_metallicTextureSelector, &Material::metallicTexture);
    setTextureUpdateCallback(_emissiveTextureSelector, &Material::emissiveTexture);

    _litMaterialCheckbox.setUpdateCallback([updateConstructionFunction, materialRef](bool lit) {
      materialRef->lit = lit;
      updateConstructionFunction();
    });
  } else {
    Logger::ERROR("Cannot set up a MaterialInspector for an Asset that is not Material.");
  }
}

void InspectorWidget::MaterialInspector::draw() {
  utils::ui::drawCenteredTitle("Albedo");
  _litMaterialCheckbox.draw();
  _albedoColorSelector.draw();
  _albedoTextureSelector.draw();

  ImGui::Separator();

  utils::ui::drawCenteredTitle("Normal");
  _normalMapStrengthSlider.draw();
  _normalTextureSelector.draw();

  ImGui::Separator();

  utils::ui::drawCenteredTitle("Roughness");
  _roughnessSlider.draw();
  _roughnessTextureSelector.draw();

  ImGui::Separator();

  utils::ui::drawCenteredTitle("Metallic");
  _metallicSlider.draw();
  _metallicTextureSelector.draw();

  ImGui::Separator();

  utils::ui::drawCenteredTitle("Emissive");
  _emissiveColorSelector.draw();
  _emissiveStrengthSlider.draw();
  _emissiveTextureSelector.draw();
}

InspectorWidget::TextureInspector::TextureInspector(Registry* registry, AssetHandle handle) {
  if (registry->hasComponent<AssetComponent<Texture>>(handle)) {
    auto& textureAsset = registry->getComponent<AssetComponent<Texture>>(handle);
    _texture = utils::assets::read(textureAsset);
  } else {
    Logger::ERROR("Cannot set up a TextureInspector for an Asset that is not Texture.");
  }
}

void InspectorWidget::TextureInspector::draw() {
  if (_texture) {
    const auto textureSize = _texture->getSize();

    if (textureSize.x > 0 && textureSize.y > 0) {
      const auto availableRegionSize = ImGui::GetContentRegionAvail();
      const auto availableWidth = availableRegionSize.x;
      const auto availableHeight = availableRegionSize.y / 3;

      const auto textureAspectRatio = static_cast<float>(textureSize.y) / textureSize.x;
      const auto availableAspectRatio = static_cast<float>(availableHeight) / availableWidth;

      ImVec2 imageSize;
      auto offsetX = 0.0f;

      if (textureAspectRatio > availableAspectRatio) {
        imageSize.y = availableHeight;
        imageSize.x = imageSize.y / textureAspectRatio;

        offsetX = (availableWidth - imageSize.x) * 0.5f;
      } else {
        imageSize.x = availableWidth;
        imageSize.y = imageSize.x * textureAspectRatio;
      }

      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
      ImGui::Image(_texture->getImguiTextureID(), imageSize);
    }
  }
}

}  // namespace shkyera
