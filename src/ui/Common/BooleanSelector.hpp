#pragma once

#include <functional>
#include <string>

#include <glm/glm.hpp>
#include "imgui.h"

namespace shkyera {

class BooleanSelector {
   public:
    BooleanSelector(const std::string& title, bool value);

    void setUpdateCallback(std::function<void(bool value)> callback);

    void draw();

   private:
    std::string _title;
    std::string _imguiIdentifier;
    bool _value;
    std::function<void(bool value)> _updateCallback;
};

}  // namespace shkyera
