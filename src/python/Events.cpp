#include "python/Events.hpp"
#include "ui/widgets/ConsoleWidget.hpp"

#include <iostream>
#include <numeric>

namespace shkyera::Python {

std::shared_ptr<Renderer> _eventRenderer;

void setRenderer(std::shared_ptr<Renderer> renderer) { _eventRenderer = renderer; }

template <typename T> std::vector<T> parsePayload(py::list payload) {
    try {
        std::vector<T> parsed;

        for (py::handle obj : payload) {
            parsed.push_back(obj.cast<T>());
        }

        return parsed;
    } catch (std::exception &ex) {
        ConsoleWidget::logError("Could not parse payload of an event.");
        return {};
    }
}

template <Event event> void processEvent(py::list payload) {
    std::vector<std::string> message = parsePayload<std::string>(payload);
    std::string concatenatedMessage = std::accumulate(message.begin(), message.end(), std::string(""));
    ConsoleWidget::logError("Handling of event " + std::to_string(event) + " with payload " + concatenatedMessage +
                            " is not implemented.");
}

template <> void processEvent<LOG_INFO>(py::list payload) {
    std::vector<std::string> message = parsePayload<std::string>(payload);
    ConsoleWidget::logInfo(std::accumulate(message.begin(), message.end(), std::string("")));
}
template <> void processEvent<LOG_ERROR>(py::list payload) {
    std::vector<std::string> message = parsePayload<std::string>(payload);
    ConsoleWidget::logError(std::accumulate(message.begin(), message.end(), std::string("")));
}
template <> void processEvent<LOG_SUCCESS>(py::list payload) {
    std::vector<std::string> message = parsePayload<std::string>(payload);
    ConsoleWidget::logSuccess(std::accumulate(message.begin(), message.end(), std::string("")));
}
template <> void processEvent<LOG_VERBOSE>(py::list payload) {
    std::vector<std::string> message = parsePayload<std::string>(payload);
    ConsoleWidget::logVerbose(std::accumulate(message.begin(), message.end(), std::string("")));
}

template <> void processEvent<DRAW_LINE>(py::list payload) {
    auto coords = parsePayload<int>(payload);
    int x1 = coords[0], y1 = coords[1], x2 = coords[2], y2 = coords[3];

    _eventRenderer->drawLine(x1, y1, x2, y2);
}

} // namespace shkyera::Python
