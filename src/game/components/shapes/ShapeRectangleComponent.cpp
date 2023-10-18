#include "game/components/shapes/ShapeRectangleComponent.hpp"

#include <iostream>

namespace shkyera {

void ShapeRectangleComponent::drawShape() {
    std::shared_ptr<GameObject> object = getObject();

    glm::vec3 position = object->getPosition();

    _renderer->drawRectangle(position[0], position[1], (int)_width, (int)_height, _color);
}

void ShapeRectangleComponent::setWidth(float w) { _width = w; }
void ShapeRectangleComponent::setHeight(float h) { _height = h; }
void ShapeRectangleComponent::setColor(glm::vec3 color) { _color = color; }

} // namespace shkyera
