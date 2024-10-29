#pragma once

#include <functional>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace shkyera {

class InputManager{
    public:
        enum class CoordinateSystem {
            ABSOLUTE,
            SCENE
        };

        using Key = int;
        using MouseButton = int;

        static InputManager& getInstance();

        void setCoordinateSystem(CoordinateSystem system, glm::vec2 topLeftCorner, glm::vec2 bottomRightCorner);

        glm::vec2 getRelativeMousePosition(CoordinateSystem system);
        glm::vec2 getMousePosition(CoordinateSystem system);

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

        std::array<bool, 10> _previousMouseButtonStates = { false };
        glm::vec2 _latestMousePosition;
        std::unordered_map<CoordinateSystem, std::pair<glm::vec2, glm::vec2>> _coordinateSystems;

        std::unordered_map<Key, std::vector<std::function<void()>>> _keyCallbacks;
        std::vector<std::function<void(double, double)>> _mouseMoveCallbacks;
        std::unordered_map<MouseButton, std::vector<std::function<void()>>> _mouseButtonDownCallbacks;
        std::unordered_map<MouseButton, std::vector<std::function<void()>>> _mouseButtonUpCallbacks;
};

}
