#pragma once

#include <functional>

#include <GLFW/glfw3.h>

namespace shkyera {

class InputManager{
    public:
        using Key = int;
        using MouseButton = int;

        static InputManager& getInstance();

        void registerKeyCallback(Key key, std::function<void()> callback);

        void unregisterKeyCallback(Key key);

        void registerMouseMoveCallback(std::function<void(double, double)> callback);

        void unregisterMouseMoveCallback();

        void registerMouseButtonDownCallback(MouseButton button, std::function<void()> callback);
        
        void unregisterMouseButtonDownCallback(MouseButton button);
        
        void registerMouseButtonUpCallback(MouseButton button, std::function<void()> callback);
        
        void unregisterMouseButtonUpCallback(MouseButton button);

        void processInput(GLFWwindow* window);

    private:
        InputManager() = default;
        void processMouseButton(GLFWwindow* window, MouseButton button);
        void onMouseButtonDown(MouseButton button);
        void onMouseButtonUp(MouseButton button);

        std::unordered_map<Key, std::vector<std::function<void()>>> _keyCallbacks;
        std::vector<std::function<void(double, double)>> _mouseMoveCallbacks;
        std::unordered_map<MouseButton, std::vector<std::function<void()>>> _mouseButtonDownCallbacks;
        std::unordered_map<MouseButton, std::vector<std::function<void()>>> _mouseButtonUpCallbacks;
        std::array<bool, 10> _previousMouseButtonStates = { false };
};

}
