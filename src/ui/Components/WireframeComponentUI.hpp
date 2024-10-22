#pragma once

#include <UI/ComponentUI.hpp>
#include <UI/Common/FileSelector.hpp>
#include <Components/WireframeComponent.hpp>

namespace shkyera {

class WireframeComponentUI : public ComponentUI {
public:
    WireframeComponentUI(WireframeComponent* wireframeComponent);

    void draw() override;

private:
    WireframeComponent* _wireframeComponent;
    FileSelector _wireframeSelector;
};

}
