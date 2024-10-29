#pragma once

#include <memory>
#include <ECS/Registry.hpp>

namespace shkyera {

class ObjectSelectionSystem {
public:
    ObjectSelectionSystem(std::shared_ptr<Registry> registry);

    void setOnSelectCallback(std::function<void(Entity)> callback);

private:
    void tryToSelectObject();

    std::function<void(Entity)> _onSelectCallback;
    std::shared_ptr<Registry> _registry;
};

}
