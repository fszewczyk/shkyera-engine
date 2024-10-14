#include <InputManager/InputManager.hpp>

namespace shkyera {

InputManager& InputManager::getInstance() {
    static InputManager manager;
    return manager;
}

void InputManager::registerKeyCallback(int key, std::function<void()> callback) {
    _keyCallbacks[key].push_back(callback);
}

void InputManager::unregisterKeyCallback(int key) {
    _keyCallbacks.erase(key);
}

void InputManager::registerMouseMoveCallback(std::function<void(double, double)> callback) {
    _mouseCallbacks.push_back(callback);
}

void InputManager::unregisterMouseMoveCallback() {
    _mouseCallbacks.clear();
}

void InputManager::processInput(GLFWwindow* window) {
    for (auto& [key, callbacks] : _keyCallbacks) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            for (auto& callback : callbacks) {
                callback();
            }
        }
    }

    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    for (auto& callback : _mouseCallbacks) {
        callback(xPos, yPos);
    }
}

}
