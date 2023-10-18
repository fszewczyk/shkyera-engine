#include "game/components/shapes/ShapeCircleComponent.hpp"

#include <iostream>

namespace shkyera {

void ShapeCircleComponent::drawShape() {
    std::shared_ptr<GameObject> object = getObject();

    glm::vec3 position = object->getPosition();

    _renderer->drawCircle(position[0], position[1], (int)_radius);
}

void ShapeCircleComponent::setRadius(float r) { _radius = r; }

} // namespace shkyera
