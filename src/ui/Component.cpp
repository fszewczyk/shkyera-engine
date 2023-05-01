#include "ui/Component.hpp"

namespace shkyera {

UIComponent::UIComponent(std::string name, std::shared_ptr<GameObject> object) : _name(name), _object(object) {}

void UIComponent::addComponentToObject(std::shared_ptr<GameObject> object, std::shared_ptr<UIComponent> component) {
    if (_objectComponents.find(object) == _objectComponents.end())
        _objectComponents.insert({object, {}});

    _objectComponents[object].push_back(component);
}

std::vector<std::shared_ptr<UIComponent>> UIComponent::getComponentsOfObject(std::shared_ptr<GameObject> object) {
    if (_objectComponents.find(object) == _objectComponents.end())
        return {};
    return _objectComponents[object];
}

std::unordered_map<std::shared_ptr<GameObject>, std::vector<std::shared_ptr<UIComponent>>>
    UIComponent::_objectComponents = {};

} // namespace shkyera