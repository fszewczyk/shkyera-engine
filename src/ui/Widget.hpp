#pragma once

#include <GLFW/glfw3.h>
#include <cmath>

#include <string>

namespace shkyera {

class Widget {
  public:
    Widget(std::string name);

    virtual void draw() const = 0;

    std::string name;
};

} // namespace shkyera
