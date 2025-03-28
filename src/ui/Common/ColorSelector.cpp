#include "imgui.h"

#include <UI/Common/ColorSelector.hpp>
#include <UI/Common/Style.hpp>

namespace shkyera {

ColorSelector::ColorSelector(const std::string& title) : _title(title), _imguiIdentifier("##" + _title + "ColorSelector") {}

ColorSelector::ColorSelector(const std::string& title, const glm::vec3& color) : _title(title), _imguiIdentifier("##" + _title + "ColorSelector"), _color(color) {}

void ColorSelector::setUpdateCallback(std::function<void(const glm::vec3& color)> callback) {
    _updateCallback = callback;
}

void ColorSelector::draw() {
    std::array<float, 3> newColor = { _color.x, _color.y, _color.z };

    ImGui::PushItemWidth(50);
    ImGui::TextUnformatted(_title.c_str());
    ImGui::PopItemWidth();

    ImGui::SameLine(130);
    ImGui::PushItemWidth(190);
    ImGui::ColorEdit3(_imguiIdentifier.c_str(), newColor.data(), ImGuiColorEditFlags_PickerHueBar);
    ImGui::PopItemWidth();

    if(newColor[0] != _color.x || newColor[0] != _color.y || newColor[0] != _color.y)
    {
        _color = glm::vec3{newColor[0], newColor[1], newColor[2]};
        if(_updateCallback)
        {
            _updateCallback(_color);
        }
    }
}

}
