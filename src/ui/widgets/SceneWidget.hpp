/**
 * @file SceneWidget.hpp
 *
 * @brief Contains the declaration of the `SceneWidget` class, a user interface widget for rendering scenes.
 */

#pragma once

#include <Runtime/Runtime.hpp>

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

    SceneWidget(std::shared_ptr<Registry> registry);

    /**
     * @brief Implementation of the abstract `draw` method to render the scene widget.
     */
    virtual void draw() override;

    /**
     * @brief Adjust the size of the scene widget.
     */
    void adjustSize();

  private:
    void drawScene();

    Runtime _runtime;
    ImVec2 _renderSize;                  ///< The size of the rendered scene area.
};

} // namespace shkyera
