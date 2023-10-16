#include "game/Component.hpp"

namespace shkyera {

Component::Component(std::shared_ptr<GameObject> object) : _object(object) {}

std::shared_ptr<GameObject> Component::getObject() { return _object; }

} // namespace shkyera
