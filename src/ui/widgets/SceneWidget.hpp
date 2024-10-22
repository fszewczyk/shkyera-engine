/**
 * @file SceneWidget.hpp
 *
 * @brief Contains the declaration of the `SceneWidget` class, a user interface widget for rendering scenes.
 */

#pragma once

#include <Rendering/ShaderProgram.hpp>
#include <Runtime/Runtime.hpp>
#include <UI/Widget.hpp>
#include <GLFW/glfw3.h>
#include <memory>

namespace shkyera {

/**
 * @brief A user interface widget for rendering scenes.
 *
 * The `SceneWidget` class provides a graphical user interface widget for rendering and contrlling the runtime
 */
class SceneWidget : public Widget {
  public:
    using Widget::Widget;

    SceneWidget(std::shared_ptr<Registry> registry);

    /**
     * @brief Implementation of the abstract `draw` method to render the scene widget.
     */
    virtual void draw() override;

  private:
    Runtime _runtime;
};

} // namespace shkyera
