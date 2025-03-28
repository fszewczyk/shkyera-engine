#pragma once

#include <memory>
#include <ECS/Registry.hpp>

namespace shkyera {

class ObjectSelectionSystem {
public:
    ObjectSelectionSystem(std::shared_ptr<Registry> registry);

private:
    std::optional<Entity> getHoveredObject();

    std::optional<Entity> _objectSelectedOnMouseDown;
    std::shared_ptr<Registry> _registry;
};

}
