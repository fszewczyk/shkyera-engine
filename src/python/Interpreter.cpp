#include "python/Interpreter.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

#include <iostream>

namespace shkyera::Python {

py::module_ logger;
py::module_ main;

void runLogger() {
    py::list info = logger.attr("get_info_logs")();
    py::list error = logger.attr("get_error_logs")();
    py::list success = logger.attr("get_success_logs")();
    py::list verbose = logger.attr("get_verbose_logs")();

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

    logger.attr("clear")();
}

void initialize() {
    logger = py::module_::import("src.python.shkyera.logger");
    main = py::module_::import("src.python.shkyera.shkyera");
}

void run() {
    py::scoped_interpreter guard{};

    try {
        initialize();
        main.attr("update")();
        runLogger();
    } catch (std::exception &error) {
        throw std::string(error.what());
    }
}

} // namespace shkyera::Python
