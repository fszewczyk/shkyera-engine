#pragma once

#include <Utils/AssetUtils.hpp>
#include <AssetManager/Texture.hpp>

namespace shkyera {

class ComponentUI {
    public:
        ComponentUI() {
            _icon = utils::assets::readPermanent<Texture>(Image::ICON_COMPONENT_TRANSFORM);
        }

        virtual ~ComponentUI() = default;

        virtual void draw() = 0;

    protected:
        AssetRef<Texture> _icon;
};

}