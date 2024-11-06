#include "imgui.h"

#include <UI/Common/FloatSlider.hpp>
#include <UI/Common/Style.hpp>

namespace shkyera {

FloatSlider::FloatSlider(const std::string& title, float min, float max) : _title(title), _imguiIdentifier("##" + _title + "Slider"), _value((min + max) / 2), _minimum(min), _maximum(max) {}

FloatSlider::FloatSlider(const std::string& title, float value, float min, float max) : _title(title), _imguiIdentifier("##" + _title + "Slider"), _value(value), _minimum(min), _maximum(max) {}

void FloatSlider::setUpdateCallback(std::function<void(float value)> callback) {
    _updateCallback = callback;
}

void FloatSlider::draw() {
    float oldValue = _value;
    
    ImGui::PushItemWidth(50);
    ImGui::TextUnformatted(_title.c_str());
    ImGui::PopItemWidth();

    ImGui::SameLine(120);
    ImGui::PushItemWidth(190);
    ImGui::SliderFloat(_imguiIdentifier.c_str(), &_value, _minimum, _maximum, "%.3f");
    ImGui::PopItemWidth();

    if(oldValue != _value)
    {
        if(_updateCallback)
        {
            _updateCallback(_value);
        }
    }
}

}
