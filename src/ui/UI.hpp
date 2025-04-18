/**
 * @file UI.hpp
 *
 * @brief Contains the declaration of the `UI` class, responsible for handling the user interface.
 */

#pragma once

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <ECS/Registry.hpp>
#include <Systems/GizmoSystem.hpp>
#include <Systems/ObjectSelectionSystem.hpp>
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
     */
  UI();

  /**
     * @brief Initialize the systems and widgets
     *
     * @param registry A shared pointer to the registry that the UI will configure.
     */
  void initialize(std::shared_ptr<Registry> registry);

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

  std::shared_ptr<Registry> _registry;            ///< A shared pointer to the associated game.
  std::vector<std::unique_ptr<Widget>> _widgets;  ///< A collection of UI widgets.

  bool _open;           ///< Flag indicating if the UI is open.
  GLFWwindow* _window;  ///< The UI window.
};

}  // namespace shkyera
