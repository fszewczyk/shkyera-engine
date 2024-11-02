#pragma once

#include <memory>
#include <ECS/Registry.hpp>

namespace shkyera {

class ObjectSelectionSystem {
public:
    ObjectSelectionSystem(std::shared_ptr<Registry> registry);

private:
    void tryToSelectObject();

    std::shared_ptr<Registry> _registry;
};

}
