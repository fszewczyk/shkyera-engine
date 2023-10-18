#include "game/components/shapes/ShapeComponent.hpp"

namespace shkyera {

void ShapeComponent::setRenderer(std::shared_ptr<Renderer> renderer) { _renderer = renderer; }
void ShapeComponent::addShape(std::shared_ptr<ShapeComponent> shape) { _shapes.push_back(shape); }
void ShapeComponent::removeShape(std::shared_ptr<ShapeComponent> shape) { std::erase(_shapes, shape); }
std::vector<std::shared_ptr<ShapeComponent>> ShapeComponent::getShapes() { return _shapes; }

std::vector<std::shared_ptr<ShapeComponent>> ShapeComponent::_shapes = {};

} // namespace shkyera
