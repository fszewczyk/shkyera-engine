/**
 * @file SceneWidget.hpp
 *
 * @brief Contains the declaration of the `SceneWidget` class, a user interface widget for rendering scenes.
 */

#pragma once

#include <UI/Widget.hpp>

namespace shkyera {

/**
 * @brief A user interface widget for rendering scenes.
 *
 * The `SceneWidget` class provides a graphical user interface widget for rendering and displaying scenes using a
 * renderer.
 */
class SceneWidget : public Widget {
 public:
  using Widget::Widget;

  /**
     * @brief Implementation of the abstract `draw` method to render the scene widget.
     */
  virtual void draw() override;

  /**
     * @brief Adjust the size of the scene widget.
     */
  void adjustSize();

 private:
  /**
     * @brief Draw the runtime rendered scene.
     */
  void drawRuntime() const;

  /**
     * @brief Draw the empty scene when it is not running.
     */
  void drawScene();

  /**
     * @brief Read the user input from the keyboard and notify the Python intepreter of it.
     */
  void readInput() const;

  ImVec2 _renderSize;  ///< The size of the rendered scene area.
};

}  // namespace shkyera
