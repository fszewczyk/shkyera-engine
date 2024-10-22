#pragma once

#include <UI/ComponentUI.hpp>

#include <UI/Common/FileSelector.hpp>
#include <Components/ModelComponent.hpp>

namespace shkyera {

class ModelComponentUI : public ComponentUI {
    public:
        ModelComponentUI(ModelComponent* modelComponent);

        void draw() override;

    private:
        ModelComponent* _modelComponent;
        FileSelector _meshSelector;
        FileSelector _materialSelector;
};

}
