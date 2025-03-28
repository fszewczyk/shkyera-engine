/**
 * @file SceneWidget.hpp
 *
 * @brief Contains the declaration of the `SceneWidget` class, a user interface widget for rendering scenes.
 */

#pragma once

#include <memory>

#include <Common/Types.hpp>
#include <Rendering/ShaderProgram.hpp>
#include <Runtime/Runtime.hpp>
#include <UI/Widget.hpp>
#include <GLFW/glfw3.h>

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
    void updateWindowCoordinateSystem();

    std::shared_ptr<Registry> _registry;
    Runtime<RuntimeMode::DEVELOPMENT> _runtime;
};

} // namespace shkyera
