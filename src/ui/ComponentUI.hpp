#pragma once

#include <AssetManager/Texture.hpp>
#include <Utils/AssetUtils.hpp>

namespace shkyera {

class ComponentUI {
 public:
  ComponentUI() { _icon = utils::assets::readPermanent<Texture>(Image::ICON_COMPONENT_TRANSFORM); }

  ComponentUI(const std::filesystem::path& texturePath) { _icon = utils::assets::readPermanent<Texture>(texturePath); }

  virtual ~ComponentUI() = default;

  virtual void draw() = 0;

 protected:
  AssetRef<Texture> _icon;
};

}  // namespace shkyera