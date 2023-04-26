#pragma once

#include <GLFW/glfw3.h>
#include <cmath>

#include <string>

namespace shkyera {

class Widget {
  public:
    Widget(std::string name);

    virtual void draw() = 0;

    std::string _name;
};

} // namespace shkyera
