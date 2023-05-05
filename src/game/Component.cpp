#include "game/Component.hpp"

namespace shkyera {

Component::Component(std::shared_ptr<GameObject> object) : _object(object) {}

} // namespace shkyera
