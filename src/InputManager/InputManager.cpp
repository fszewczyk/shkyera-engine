#include <InputManager/InputManager.hpp>

namespace shkyera {

InputManager& InputManager::getInstance() {
    static InputManager manager;
    return manager;
}

void InputManager::registerKeyCallback(Key key, std::function<void()> callback) {
    _keyCallbacks[key].push_back(callback);
}

void InputManager::unregisterKeyCallback(Key key) {
    _keyCallbacks.erase(key);
}

void InputManager::registerMouseMoveCallback(std::function<void(double, double)> callback) {
    _mouseMoveCallbacks.push_back(callback);
}

void InputManager::unregisterMouseMoveCallback() {
    _mouseMoveCallbacks.clear();
}

void InputManager::registerMouseButtonUpCallback(MouseButton button, std::function<void()> callback) {
    _mouseButtonUpCallbacks[button].push_back(callback);
}

void InputManager::unregisterMouseButtonUpCallback(MouseButton button) {
    _mouseButtonUpCallbacks.erase(button);
}

void InputManager::registerMouseButtonDownCallback(MouseButton button, std::function<void()> callback) {
    _mouseButtonDownCallbacks[button].push_back(callback);
}

void InputManager::unregisterMouseButtonDownCallback(MouseButton button) {
    _mouseButtonDownCallbacks.erase(button);
}

void InputManager::processInput(GLFWwindow* window) {
    for (const auto& [key, callbacks] : _keyCallbacks) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            for (const auto& callback : callbacks) {
                callback();
            }
        }
    }

    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    for (const auto& callback : _mouseMoveCallbacks) {
        callback(xPos, yPos);
    }

    processMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    processMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    processMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
}

void InputManager::processMouseButton(GLFWwindow* window, MouseButton button) {
    int mouseButtonState = glfwGetMouseButton(window, button);
    if(mouseButtonState != _previousMouseButtonStates[button]) {
        if(mouseButtonState == GLFW_RELEASE) {
            onMouseButtonUp(button);
        } else {
            onMouseButtonDown(button);
        }
        _previousMouseButtonStates[button] = mouseButtonState;
    }
}


void InputManager::onMouseButtonDown(MouseButton button) {
    if(_mouseButtonDownCallbacks.count(button) == 0) {
        return;
    }

    for(const auto& callback : _mouseButtonDownCallbacks[button]) {
        callback();
    }
}

void InputManager::onMouseButtonUp(MouseButton button) {
    if(_mouseButtonUpCallbacks.count(button) == 0) {
        return;
    }
    
    for(const auto& callback : _mouseButtonUpCallbacks[button]) {
        callback();
    }
}

}
