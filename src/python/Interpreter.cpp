#include "python/Interpreter.hpp"
#include "game/components/ScriptComponent.hpp"
#include "python/Events.hpp"

#include <iostream>
#include <mutex>
#include <thread>

namespace shkyera::Python {

std::mutex runningMutex;

bool _currentlyRunning = false;
bool _canRun = false;

py::object _game;
py::object _eventSystem;

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
}

void initialize() {
    _game = py::module_::import((MODULE + "game").c_str()).attr("Game")();
    std::vector<std::shared_ptr<ScriptComponent>> customScripts = ScriptComponent::getScripts();
    for (std::shared_ptr<ScriptComponent> script : customScripts) {
        std::string modulePath = "resources.scripts." + script->getFile()->getNameWithoutExtension();
        std::cerr << modulePath << '\n';
        py::object customObject = py::module_::import(modulePath.c_str()).attr("Object")();
        _game.attr("add_object")(customObject);
    }

    _eventSystem = py::module_::import((MODULE + "events").c_str());

    _eventHandlers[LOG_INFO] = &processEvent<LOG_INFO>;
    _eventHandlers[LOG_ERROR] = &processEvent<LOG_ERROR>;
    _eventHandlers[LOG_SUCCESS] = &processEvent<LOG_SUCCESS>;
    _eventHandlers[LOG_VERBOSE] = &processEvent<LOG_VERBOSE>;

    _eventHandlers[DRAW_LINE] = &processEvent<DRAW_LINE>;
    _eventHandlers[DRAW_CLEAR] = &processEvent<DRAW_CLEAR>;
}

void runGame() { _game.attr("update")(); }

void startImplicit() {
    py::scoped_interpreter guard{};

    initialize();

    while (_currentlyRunning) {
        if (_canRun) {
            runGame();
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

} // namespace shkyera::Python
