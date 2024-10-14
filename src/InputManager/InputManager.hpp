#pragma once

#include <functional>

#include <GLFW/glfw3.h>

namespace shkyera {

class InputManager{
    public:
        static InputManager& getInstance();

        void registerKeyCallback(int key, std::function<void()> callback);

        void unregisterKeyCallback(int key);

        void registerMouseMoveCallback(std::function<void(double, double)> callback);

        void unregisterMouseMoveCallback();

        void processInput(GLFWwindow* window);

    private:
        InputManager() = default;

        std::unordered_map<int, std::vector<std::function<void()>>> _keyCallbacks;
        std::vector<std::function<void(double, double)>> _mouseCallbacks;
};

}
