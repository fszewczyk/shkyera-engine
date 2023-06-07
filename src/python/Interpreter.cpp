#include "python/Interpreter.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

#include <iostream>
#include <thread>

namespace shkyera::Python {

py::object _game;
py::object _logger;

void runLogger() {
    py::list info = _logger.attr("get_info_logs")();
    py::list error = _logger.attr("get_error_logs")();
    py::list success = _logger.attr("get_success_logs")();
    py::list verbose = _logger.attr("get_verbose_logs")();

    for (py::handle message : info) {
        std::string toLog = message.attr("__str__")().cast<std::string>();
        ConsoleWidget::logInfo(toLog);
    }

    for (py::handle message : error) {
        std::string toLog = message.attr("__str__")().cast<std::string>();
        ConsoleWidget::logError(toLog);
    }

    for (py::handle message : success) {
        std::string toLog = message.attr("__str__")().cast<std::string>();
        ConsoleWidget::logSuccess(toLog);
    }

    for (py::handle message : verbose) {
        std::string toLog = message.attr("__str__")().cast<std::string>();
        ConsoleWidget::logVerbose(toLog);
    }

    _logger.attr("clear")();
}

void initialize() {
    _game = py::module_::import("src.python.shkyera.game").attr("Game")();
    _logger = py::module_::import("src.python.shkyera.game");
}

void runGame() { _game.attr("update")(); }

void startImplicit() {
    py::scoped_interpreter guard{};

    initialize();
    while (true) {
        runGame();
        runLogger();
    }
}

void start() {
    std::thread game{startImplicit};
    game.detach();
    // game.join();
}

} // namespace shkyera::Python
