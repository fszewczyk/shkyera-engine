#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>

namespace shkyera {

class InputManager {
 public:
  enum class CoordinateSystem { ABSOLUTE, SCENE, RUNTIME };

  using Key = int;
  using MouseButton = int;

  static InputManager& getInstance();

  void setWindow(GLFWwindow* window);
  void setCoordinateSystem(CoordinateSystem system, glm::vec2 topLeftCorner, glm::vec2 bottomRightCorner);
  void update();

  // Key queries
  bool isKeyDown(Key key) const;      // held this frame
  bool isKeyUp(Key key) const;        // not held this frame
  bool isKeyPressed(Key key) const;   // went down this frame
  bool isKeyReleased(Key key) const;  // went up this frame

  // Mouse button queries
  bool isMouseButtonDown(MouseButton button) const;
  bool isMouseButtonUp(MouseButton button) const;
  bool isMouseButtonPressed(MouseButton button) const;
  bool isMouseButtonReleased(MouseButton button) const;

  // Mouse position
  glm::vec2 getMousePosition(CoordinateSystem coordinateSystem) const;
  glm::vec2 getRelativeMousePosition(CoordinateSystem coordinateSystem) const;
  bool isMouseInside(CoordinateSystem coordinteSystem) const;

 private:
  InputManager();

  static constexpr int MAX_KEYS = GLFW_KEY_LAST + 1;
  static constexpr int MAX_MOUSE_BUTTONS = GLFW_MOUSE_BUTTON_LAST + 1;

  GLFWwindow* _window = nullptr;

  std::array<bool, MAX_KEYS> _currentKeyStates{};
  std::array<bool, MAX_KEYS> _previousKeyStates{};

  std::array<bool, MAX_MOUSE_BUTTONS> _currentMouseStates{};
  std::array<bool, MAX_MOUSE_BUTTONS> _previousMouseStates{};

  glm::vec2 _mousePos{0.0f, 0.0f};

  std::unordered_map<CoordinateSystem, std::pair<glm::vec2, glm::vec2>> _coordinateSystems;
};

}  // namespace shkyera
