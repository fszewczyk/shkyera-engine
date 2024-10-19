#pragma once

#include "imgui.h"

#include <Components/MeshComponent.hpp>
#include <UI/ComponentUI.hpp>
#include <UI/Common/FileSelector.hpp>

namespace shkyera {

class MeshComponentUI : public ComponentUI {
    public:
        MeshComponentUI(MeshComponent* meshComponent);

        void draw() override;

    private:
        MeshComponent* _meshComponent;
        FileSelector _fileSelector;
};

}
