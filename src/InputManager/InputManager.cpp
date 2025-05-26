#include <InputManager/InputManager.hpp>
#include <algorithm>

namespace shkyera {

InputManager& InputManager::getInstance() {
  static InputManager instance;
  return instance;
}

InputManager::InputManager() {
  _currentKeyStates.fill(false);
  _previousKeyStates.fill(false);
  _currentMouseStates.fill(false);
  _previousMouseStates.fill(false);
}

void InputManager::setWindow(GLFWwindow* window) {
  _window = window;
}

void InputManager::setCoordinateSystem(CoordinateSystem system, glm::vec2 topLeftCorner, glm::vec2 bottomRightCorner) {
  _coordinateSystems[system] = {topLeftCorner, bottomRightCorner};
}

void InputManager::update() {
  if (!_window)
    return;

  // Update previous states
  _previousKeyStates = _currentKeyStates;
  _previousMouseStates = _currentMouseStates;

  // Poll keys
  for (int i = 0; i < MAX_KEYS; ++i) {
    _currentKeyStates[i] = (glfwGetKey(_window, i) == GLFW_PRESS);
  }

  // Poll mouse buttons
  for (int b = 0; b < MAX_MOUSE_BUTTONS; ++b) {
    _currentMouseStates[b] = (glfwGetMouseButton(_window, b) == GLFW_PRESS);
  }

  // Update mouse position
  double x, y;
  glfwGetCursorPos(_window, &x, &y);
  _mousePos = {static_cast<float>(x), static_cast<float>(y)};
}

bool InputManager::isKeyDown(Key key) const {
  return key >= 0 && key < MAX_KEYS && _currentKeyStates[key];
}

bool InputManager::isKeyUp(Key key) const {
  return key >= 0 && key < MAX_KEYS && !_currentKeyStates[key];
}

bool InputManager::isKeyPressed(Key key) const {
  return key >= 0 && key < MAX_KEYS && !_previousKeyStates[key] && _currentKeyStates[key];
}

bool InputManager::isKeyReleased(Key key) const {
  return key >= 0 && key < MAX_KEYS && _previousKeyStates[key] && !_currentKeyStates[key];
}

bool InputManager::isMouseButtonDown(MouseButton button) const {
  return button >= 0 && button < MAX_MOUSE_BUTTONS && _currentMouseStates[button];
}

bool InputManager::isMouseButtonUp(MouseButton button) const {
  return button >= 0 && button < MAX_MOUSE_BUTTONS && !_currentMouseStates[button];
}

bool InputManager::isMouseButtonPressed(MouseButton button) const {
  return button >= 0 && button < MAX_MOUSE_BUTTONS && !_previousMouseStates[button] && _currentMouseStates[button];
}

bool InputManager::isMouseButtonReleased(MouseButton button) const {
  return button >= 0 && button < MAX_MOUSE_BUTTONS && _previousMouseStates[button] && !_currentMouseStates[button];
}

glm::vec2 InputManager::getMousePosition(CoordinateSystem system) const {
  if (_coordinateSystems.count(system) != 0) {
    const auto& [topLeft, _bottomRight] = _coordinateSystems.at(system);
    return _mousePos - topLeft;
  }
  return {0, 0};
}

glm::vec2 InputManager::getRelativeMousePosition(CoordinateSystem system) const {
  if (_coordinateSystems.count(system) != 0) {
    const auto& [topLeft, bottomRight] = _coordinateSystems.at(system);
    return (_mousePos - topLeft) / (bottomRight - topLeft);
  }
  return {0, 0};
}

bool InputManager::isMouseInside(CoordinateSystem system) const {
  const auto& mousePosition = getRelativeMousePosition(system);
  return 0.0f < mousePosition.x && 0.0f < mousePosition.y && mousePosition.x < 1.0f && mousePosition.y < 1.0f;
}

}  // namespace shkyera
