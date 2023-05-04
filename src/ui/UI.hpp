#pragma once

#include <memory>
#include <vector>

#include "imgui.h"
#include <GLFW/glfw3.h>

#include "game/Game.hpp"
#include "ui/Modal.hpp"
#include "ui/Widget.hpp"

namespace shkyera {

class UI {
  public:
    UI(std::shared_ptr<Game> game);

    void initialize();
    void draw();
    void close();

    bool shouldClose() const;

    /********        FONTS        ********/
    static ImFont *NORMAL_FONT;
    static ImFont *BIG_FONT;
    static ImFont *SMALL_FONT;

    /********        COLORS      *********/
    inline static ImVec4 BACKGROUND_COLOR = ImVec4(0.17f, 0.17f, 0.17f, 1.0f);
    inline static ImVec4 TEXT_COLOR = ImVec4(0.86f, 0.86f, 0.86f, 1.0f);
    inline static ImVec4 DISABLED_TEXT_COLOR = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    inline static ImVec4 ACCENT_COLOR = ImVec4(0.4f, 0.05f, 0.7f, 1.0f);
    inline static ImVec4 STRONG_ACCENT_COLOR = ImVec4(0.5f, 0.06f, 0.82f, 1.0f);
    inline static ImVec4 GREY = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    inline static ImVec4 LIGHT_GREY = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    inline static ImVec4 BLACK = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

  private:
    void initializeImgui();
    void initializeWidgets();
    void initializeAssets();
    void styleImgui();

    void beginFrame();
    void renderFrame();
    void endFrame();

    std::shared_ptr<Game> _game;

    bool _open;
    GLFWwindow *_window;

    std::vector<std::unique_ptr<Widget>> _widgets;
    template <typename T> Modal<T> _modal;
};

} // namespace shkyera
