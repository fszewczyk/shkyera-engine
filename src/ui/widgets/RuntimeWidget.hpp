/**
 * @file SceneWidget.hpp
 *
 * @brief Contains the declaration of the `SceneWidget` class, a user interface widget for rendering scenes.
 */

#pragma once

#include <memory>

#include <GLFW/glfw3.h>
#include <Common/Types.hpp>
#include <Components/CameraTags.hpp>
#include <Rendering/ShaderProgram.hpp>
#include <Systems/ParticleSystem.hpp>
#include <Systems/RenderingSystem.hpp>
#include <UI/Widget.hpp>

namespace shkyera {

/**
 * @brief A user interface widget for running and rendering the game runtime
 *
 * The `RuntimeWidget` class provides a graphical user interface widget for rendering and contrlling the runtime
 */
class RuntimeWidget : public Widget {
 public:
  using Widget::Widget;

  RuntimeWidget(std::shared_ptr<Registry> registry);

  /**
   * @brief Implementation of the abstract `draw` method to render the runtime widget.
   */
  virtual void draw() override;

 private:
  void updateWindowCoordinateSystem();

  std::shared_ptr<Registry> _registry;
  std::unique_ptr<Registry> _storedRegistry;

  AssetRef<Texture> _playButtonTexture;
  AssetRef<Texture> _stopButtonTexture;
};

}  // namespace shkyera
