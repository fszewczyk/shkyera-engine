#pragma once

#include <AssetManager/AssetManager.hpp>
#include <AssetManager/Image.hpp>
#include <Rendering/Texture.hpp>

namespace shkyera {

class ComponentUI {
    public:
        ComponentUI() {
            _icon = AssetManager::getInstance().getAsset<Texture>(Image::ICON_COMPONENT_TRANSFORM);
        }

        virtual ~ComponentUI() = default;

        virtual void draw() = 0;

    protected:
        TextureAsset _icon;
};

}