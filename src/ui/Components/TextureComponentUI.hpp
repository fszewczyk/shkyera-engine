#pragma once

#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <Components/TransformComponent.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class TextureComponentUI : public ComponentUI {
    public:
        void draw() override;

        void setPathGetter(std::function<std::string&(void)> getter);

    private:
        std::function<std::string&(void)> _pathGetter;
};

}
