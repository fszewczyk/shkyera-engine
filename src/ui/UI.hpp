/**
 * @file UI.hpp
 *
 * @brief Contains the declaration of the `UI` class, responsible for handling the user interface.
 */

#pragma once

#include <memory>
#include <vector>

#include "imgui.h"
#include <GLFW/glfw3.h>

#include <ECS/Registry.hpp>
#include <UI/Widget.hpp>

namespace shkyera {

/**
 * @brief A class responsible for managing the user interface (UI) of the application.
 *
 * The `UI` class is responsible for initializing and rendering the UI elements, including widgets, using ImGui.
 */
class UI {
  public:
    /**
     * @brief Constructor to create a UI manager for the specified game.
     *
     * @param registry A shared pointer to the registry that the UI will configure.
     */
    UI(std::shared_ptr<Registry> registry);

    /**
     * @brief Initialize the UI, including ImGui, widgets, and assets.
     */
    void initialize();

    /**
     * @brief Render the UI.
     */
    void draw();

    /**
     * @brief Close and clean up the UI.
     */
    void close();

    /**
     * @brief Check if the UI should be closed.
     *
     * @return True if the UI should be closed, false otherwise.
     */
    bool shouldClose() const;

    /********        FONTS        ********/
    static ImFont *NORMAL_FONT; ///< The normal font used in the UI.
    static ImFont *BIG_FONT;    ///< A larger font used in the UI.
    static ImFont *HUGE_FONT;   ///< An even larger font used in the UI.
    static ImFont *SMALL_FONT;  ///< A smaller font used in the UI.

    /********        COLORS      *********/
    inline static ImVec4 BACKGROUND_COLOR = ImVec4(0.17f, 0.17f, 0.17f, 1.0f);     ///< Background color.
    inline static ImVec4 TEXT_COLOR = ImVec4(0.86f, 0.86f, 0.86f, 1.0f);           ///< Text color.
    inline static ImVec4 DISABLED_TEXT_COLOR = ImVec4(0.86f, 0.93f, 0.89f, 0.28f); ///< Disabled text color.
    inline static ImVec4 ACCENT_COLOR = ImVec4(0.4f, 0.05f, 0.7f, 1.0f);           ///< Accent color.
    inline static ImVec4 STRONG_ACCENT_COLOR = ImVec4(0.5f, 0.06f, 0.82f, 1.0f);   ///< Strong accent color.
    inline static ImVec4 GREY = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);                    ///< Grey color.
    inline static ImVec4 LIGHT_GREY = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);              ///< Light grey color.
    inline static ImVec4 DARK_ACCENT = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);             ///< Dark accent color.
    inline static ImVec4 BLACK = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);                   ///< Black color.

  private:
    /**
     * @brief Initialize ImGui and other UI elements.
     */
    void initializeImgui();

    /**
     * @brief Initialize UI widgets.
     */
    void initializeWidgets();

    /**
     * @brief Initialize UI assets.
     */
    void initializeAssets();

    /**
     * @brief Initialize the Python interpreter.
     */
    void initializeInterpreter();

    /**
     * @brief Apply styling to ImGui elements.
     */
    void styleImgui();

    /**
     * @brief Begin a UI frame.
     */
    void beginFrame();

    /**
     * @brief Render the UI frame.
     */
    void renderFrame();

    /**
     * @brief End the UI frame.
     */
    void endFrame();

    std::shared_ptr<Registry> _registry;         ///< A shared pointer to the associated game.

    bool _open;          ///< Flag indicating if the UI is open.
    GLFWwindow *_window; ///< The UI window.

    std::vector<std::unique_ptr<Widget>> _widgets; ///< A collection of UI widgets.
};

} // namespace shkyera
