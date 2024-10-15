#pragma once

#include "imgui.h"

#include <AssetManager/Image.hpp>
#include <UI/ComponentUI.hpp>

namespace shkyera {

class CameraComponentUI : public ComponentUI {
    public:
        void draw() override;
};

}
