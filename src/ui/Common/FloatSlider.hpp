#pragma once

#include <string>
#include <functional>

#include <glm/glm.hpp>
#include "imgui.h"

namespace shkyera {

class FloatSlider {
    public:
        FloatSlider(const std::string& title, float min, float max);
        FloatSlider(const std::string& title, float value, float min, float max);
        FloatSlider(const std::string& title, float value, float min, float max, std::function<float(float)> transform);

        void setUpdateCallback(std::function<void(float value)> callback);

        void draw();

    private:
        std::string _title;
        float _value;
        float _minimum, _maximum;
        std::function<void(float value)> _updateCallback;
        std::function<float(float)> _transform;
};

}
