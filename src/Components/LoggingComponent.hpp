#pragma once

#include <string>
#include <Components/BaseComponent.hpp>

namespace shkyera {

/**
 * @brief Component for logging updates.
 *
 * Inherits from BaseComponent to provide logging functionality 
 * during entity updates.
 */
class LoggingComponent : public BaseComponent<LoggingComponent> {
public:
    /**
     * Initializes the LoggingComponent.
     */
    LoggingComponent() = default;

    /**
     * Outputs the current log message to the console.
     */
    void updateImpl() {
        std::cout << "[LOG] Entity Update: " << _message << std::endl;
    }

    /**
     * Updates the message to be logged during the next 
     * update.
     *
     * @param message The message to log.
     */
    void log(const std::string& message) {
        _message = message;
    }

private:
    std::string _message; /**< Holds the log message to be output. */
};

} // namespace shkyera
