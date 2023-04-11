#pragma once

#include <memory>
#include <vector>

#include <GLFW/glfw3.h>

#include "ui/Widget.hpp"

namespace shkyera {

class UI {
  public:
    UI();

    void initialize();
    void draw();
    void close();

    bool shouldClose() const;

  private:
    void initializeImgui();
    void initializeWidgets();

    void beginFrame();
    void renderFrame();
    void endFrame();

    bool _open;

    GLFWwindow *_window;

    std::vector<std::unique_ptr<Widget>> _widgets;
};

} // namespace shkyera
