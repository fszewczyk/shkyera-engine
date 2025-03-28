#include "imgui.h"

#include <UI/Common/FloatSlider.hpp>
#include <UI/Common/Style.hpp>

namespace shkyera {

FloatSlider::FloatSlider(const std::string& title, float min, float max) : _title(title), _value((min + max) / 2), _minimum(min), _maximum(max) {}

FloatSlider::FloatSlider(const std::string& title, float value, float min, float max) : _title(title), _value(value), _minimum(min), _maximum(max) {}

FloatSlider::FloatSlider(const std::string& title, float value, float min, float max, std::function<float(float)> transform) : FloatSlider(title, value, min, max) {
    _transform = transform;
}

void FloatSlider::setUpdateCallback(std::function<void(float value)> callback) {
    _updateCallback = callback;
}

void FloatSlider::draw() {
    float oldValue = _value;
    
    ImGui::PushItemWidth(50);
    ImGui::TextUnformatted(_title.c_str());
    ImGui::PopItemWidth();

    ImGui::SameLine(130);
    ImGui::PushItemWidth(190);
    ImGui::SliderFloat((std::string("##") + _title + "Slider").c_str(), &_value, _minimum, _maximum, "%.3f");
    ImGui::PopItemWidth();

    if(oldValue != _value)
    {
        float transformedValue = _transform ? _transform(_value) : _value;
        if(_updateCallback)
        {
            _updateCallback(transformedValue);
        }
    }
}

}
