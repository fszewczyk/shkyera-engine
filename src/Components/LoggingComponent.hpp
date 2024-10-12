#pragma once

#include <string>

#include <Components/BaseComponent.hpp>

namespace shkyera {

class LoggingComponent : public BaseComponent<LoggingComponent> {
public:
    LoggingComponent() = default;

    void updateImpl() {
        std::cout << "[LOG] Entity Update: " << _message << std::endl;
    }

    void log(const std::string& message) {
        _message = message;
    }

private:
    std::string _message;
};

}