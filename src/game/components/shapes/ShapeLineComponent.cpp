#include "game/components/shapes/ShapeLineComponent.hpp"

#include <iostream>

namespace shkyera {

void ShapeLineComponent::drawShape() {
    std::shared_ptr<GameObject> object = getObject();

    glm::vec3 position = object->getPosition();

    _renderer->drawLine(position[0], position[1], position[0] + _x, position[1] + _y);
}

void ShapeLineComponent::setX(float x) { _x = x; }
void ShapeLineComponent::setY(float y) { _y = y; }

} // namespace shkyera
