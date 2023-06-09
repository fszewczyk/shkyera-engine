#include "python/Interpreter.hpp"
#include "python/Events.hpp"

#include <iostream>
#include <thread>

namespace shkyera::Python {

bool _currentlyRunning = false;

py::object _game;
py::object _eventSystem;

typedef void (*eventHandler)(py::list);
std::array<eventHandler, TOTAL_EVENTS> _eventHandlers;

void runEvents() {
    py::list events = _eventSystem.attr("get")();

    for (py::handle eventRef : events) {
        std::tuple<py::object, py::object> event = eventRef.cast<std::tuple<py::object, py::object>>();

        Event type = static_cast<Event>(std::get<0>(event).cast<int>());
        py::list payload = std::get<1>(event).cast<py::list>();

        (*_eventHandlers[type])(payload);
    }

    _eventSystem.attr("clear")();
}

void initialize() {
    _game = py::module_::import("src.python.shkyera.game").attr("Game")();
    _eventSystem = py::module_::import("src.python.shkyera.events");

    _eventHandlers[LOG_INFO] = &processEvent<LOG_INFO>;
    _eventHandlers[LOG_ERROR] = &processEvent<LOG_ERROR>;
    _eventHandlers[LOG_SUCCESS] = &processEvent<LOG_SUCCESS>;
    _eventHandlers[LOG_VERBOSE] = &processEvent<LOG_VERBOSE>;
}

void runGame() { _game.attr("update")(); }

void startImplicit() {
    py::scoped_interpreter guard{};

    initialize();
    while (_currentlyRunning) {
        runGame();
        runEvents();
    }
}

void start() {
    if (!_currentlyRunning) {
        _currentlyRunning = true;
        std::thread game{startImplicit};
        game.detach();
    }
}

void stop() { _currentlyRunning = false; }

} // namespace shkyera::Python
