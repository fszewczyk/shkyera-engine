#include "python/Interpreter.hpp"
#include "game/Component.hpp"
#include "game/GameObject.hpp"
#include "game/components/ScriptComponent.hpp"
#include "python/Events.hpp"

#include <iostream>
#include <mutex>
#include <thread>

namespace shkyera::Python {

std::mutex runningMutex;
std::mutex inputMutex;

std::vector<std::string> _pressedButtons;

bool _currentlyRunning = false;
bool _canRun = false;

py::object _game;
py::object _eventSystem;
py::object _inputSystem;

std::shared_ptr<Renderer> _renderer;

typedef void (*eventHandler)(py::list);
std::array<eventHandler, TOTAL_EVENTS> _eventHandlers;

void setInterpreterRenderer(std::shared_ptr<Renderer> renderer) { _renderer = renderer; }

void runEvents() {
    py::list events = _eventSystem.attr("get")();

    for (py::handle eventRef : events) {
        std::tuple<py::object, py::object> event = eventRef.cast<std::tuple<py::object, py::object>>();

        Event type = static_cast<Event>(std::get<0>(event).cast<int>());
        py::list payload = std::get<1>(event).cast<py::list>();

        if (type >= 0 || type < TOTAL_EVENTS)
            (*_eventHandlers[type])(payload);
    }

    _eventSystem.attr("clear")();
    _inputSystem.attr("clear")();
}

void setPublicVariables(py::object object, std::shared_ptr<ScriptComponent> script) {
    for (const PublicFloat &v : script->getFloatVariables())
        object.attr(v.name.c_str()) = v.value;
    for (const PublicInt &v : script->getIntVariables())
        object.attr(v.name.c_str()) = v.value;
    for (const PublicString &v : script->getStringVariables())
        object.attr(v.name.c_str()) = v.value;
}

void initialize() {
    _game = py::module_::import((MODULE + "lib.game").c_str()).attr("Game")();
    std::vector<std::shared_ptr<ScriptComponent>> customScripts = ScriptComponent::getScripts();
    for (std::shared_ptr<ScriptComponent> script : customScripts) {
        std::string modulePath = MODULE + script->getFile()->getNameWithoutExtension();

        std::shared_ptr<GameObject> object = script->getObject();
        glm::vec3 position = object->getPosition();
        glm::vec3 orientation = object->getOrientation();
        glm::vec3 scale = object->getScale();
        std::string name = object->getName();

        // clang-format off
        py::object customObject = py::module_::import(modulePath.c_str()).attr("Object")(
            name,
            position[0],
            position[1],
            position[2],
            orientation[0],
            orientation[1],
            orientation[2],
            scale[0],
            scale[1],
            scale[2]
        );
        // clang-format on

        setPublicVariables(customObject, script);

        _game.attr("add_object")(customObject);
    }

    _eventSystem = py::module_::import((MODULE + "lib.events").c_str());
    _eventHandlers[LOG_INFO] = &processEvent<LOG_INFO>;
    _eventHandlers[LOG_ERROR] = &processEvent<LOG_ERROR>;
    _eventHandlers[LOG_SUCCESS] = &processEvent<LOG_SUCCESS>;
    _eventHandlers[LOG_VERBOSE] = &processEvent<LOG_VERBOSE>;
    _eventHandlers[DRAW_LINE] = &processEvent<DRAW_LINE>;
    _eventHandlers[DRAW_CIRCLE] = &processEvent<DRAW_CIRCLE>;
    _eventHandlers[DRAW_CLEAR] = &processEvent<DRAW_CLEAR>;

    _inputSystem = py::module_::import((MODULE + "lib.input").c_str());

    _game.attr("setup")();
}

void runGame() {
    for (std::string key : _pressedButtons)
        _inputSystem.attr("add_key")(key);
    _game.attr("update")();
}

void startImplicit() {
    py::scoped_interpreter guard{};

    initialize();

    while (_currentlyRunning) {
        if (_canRun) {
            runGame(); // TODO: game can run always, events should wait for UI
            runEvents();

            runningMutex.lock();
            _canRun = false;
            runningMutex.unlock();
        }
    }
}

void start() {
    if (!_currentlyRunning) {
        _currentlyRunning = true;
        std::thread game{startImplicit};
        game.detach();
    }
}

void allowRunning() {
    runningMutex.lock();
    _canRun = true;
    runningMutex.unlock();
}

void stop() { _currentlyRunning = false; }

bool isRunning() { return _currentlyRunning; }

void resetPressedButtons() {
    inputMutex.lock();
    _pressedButtons.clear();
    inputMutex.unlock();
}

void addPressedButton(std::string key) {
    inputMutex.lock();
    _pressedButtons.push_back(key);
    inputMutex.unlock();
}

} // namespace shkyera::Python
