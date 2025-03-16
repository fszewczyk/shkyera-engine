#pragma once

#include <functional>
#include <map>
#include <string>

#include <glm/glm.hpp>
#include "imgui.h"

namespace shkyera {

template <typename EnumType>
class EnumSelector {
   public:
    EnumSelector(const std::string& title, EnumType value, const std::map<EnumType, std::string>& options)
        : _title(title),
          _value(value),
          _options(options) {}

    void setUpdateCallback(std::function<void(EnumType value)> callback) {
        _updateCallback = callback;
    }

    void draw() {
        EnumType oldValue = _value;
        ImGui::PushItemWidth(50);
        ImGui::TextUnformatted(_title.c_str());
        ImGui::PopItemWidth();

        ImGui::SameLine(130);
        ImGui::PushItemWidth(190);

        if (ImGui::BeginCombo((std::string("##" + _title + "EnumSelectorCombo").c_str()), _options.at(_value).c_str())) {
            for (const auto& [value, name] : _options) {
                if (ImGui::Selectable(name.c_str())) {
                    _value = value;
                }
            }

            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();

        if (oldValue != _value) {
            if (_updateCallback) {
                _updateCallback(_value);
            }
        }
    }

   private:
    std::string _title;
    EnumType _value;
    std::map<EnumType, std::string> _options;
    std::function<void(EnumType value)> _updateCallback;
};

}  // namespace shkyera
