#include "ui/widgets/ConsoleWidget.hpp"

#include <iostream>

namespace shkyera::Python {

std::string payloadToString(py::list payload) {
    std::string message = "";

    for (py::handle obj : payload) {
        message += obj.attr("__str__")().cast<std::string>();
    }

    return message;
}

template <Event event> void processEvent(py::list payload) {
    ConsoleWidget::logError("Handling of event " + std::to_string(event) + " with payload " + payloadToString(payload) +
                            " is not implemented.");
}

template <> void processEvent<LOG_INFO>(py::list payload) { ConsoleWidget::logInfo(payloadToString(payload)); }
template <> void processEvent<LOG_ERROR>(py::list payload) { ConsoleWidget::logError(payloadToString(payload)); }
template <> void processEvent<LOG_SUCCESS>(py::list payload) { ConsoleWidget::logSuccess(payloadToString(payload)); }
template <> void processEvent<LOG_VERBOSE>(py::list payload) { ConsoleWidget::logVerbose(payloadToString(payload)); }

} // namespace shkyera::Python
