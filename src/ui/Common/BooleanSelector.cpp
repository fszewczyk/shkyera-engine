#include "imgui.h"

#include <UI/Common/BooleanSelector.hpp>
#include <UI/Common/Style.hpp>

namespace shkyera {

BooleanSelector::BooleanSelector(const std::string& title, bool value)
    : _title(title), _imguiIdentifier("##" + _title + "BoolSelector"), _value(value) {}

void BooleanSelector::setUpdateCallback(std::function<void(bool value)> callback) {
  _updateCallback = callback;
}

void BooleanSelector::draw() {
  bool newValue = _value;

  ImGui::PushItemWidth(50);
  ImGui::TextUnformatted(_title.c_str());
  ImGui::PopItemWidth();

  ImGui::SameLine(130);
  ImGui::Checkbox(_imguiIdentifier.c_str(), &newValue);

  if (newValue != _value) {
    _value = newValue;
    if (_updateCallback) {
      _updateCallback(_value);
    }
  }
}

}  // namespace shkyera
