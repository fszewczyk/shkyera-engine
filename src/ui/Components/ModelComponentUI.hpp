#pragma once

#include <ECS/Registry.hpp>
#include <UI/ComponentUI.hpp>
#include <UI/Common/FileSelector.hpp>
#include <Components/ModelComponent.hpp>

namespace shkyera {

class ModelComponentUI : public ComponentUI {
    public:
        ModelComponentUI(ModelComponent* modelComponent);

        void draw() override;
        void setOnMeshUpdate(std::function<void(std::shared_ptr<Mesh>)> callback);

    private:
        ModelComponent* _modelComponent;
        FileSelector _meshSelector;
        FileSelector _materialSelector;
        std::function<void(std::shared_ptr<Mesh>)> _onMeshUpdate;
};

}
