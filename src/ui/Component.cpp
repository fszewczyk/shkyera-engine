#include "ui/Component.hpp"

namespace shkyera {

UIComponent::UIComponent(std::string name, std::shared_ptr<GameObject> object) : _name(name), _object(object) {}

} // namespace shkyera