#pragma once

#include <memory>

#include <ECS/Registry.hpp>

namespace shkyera {

class CameraMovementSystem {
public:
    CameraMovementSystem(std::shared_ptr<Registry> registry);
    void update();

private:
    void setupCameraMovement();

    bool _cameraControl;
    Entity _camera;
    std::shared_ptr<Registry> _registry;
};

}
