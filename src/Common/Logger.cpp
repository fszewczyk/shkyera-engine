#include <Common/Logger.hpp>

namespace shkyera::Logger {

static std::unordered_map<void*, Callback> _callbacks;

void ERROR(const std::string& message) {
    for (const auto& [_key, callback] : _callbacks) {
        callback(MessageType::ERROR, message);
    }
}

void INFO(const std::string& message) {
    for (const auto& [_key, callback] : _callbacks) {
        callback(MessageType::INFO, message);
    }
}

void VERBOSE(const std::string& message) {
    for (const auto& [_key, callback] : _callbacks) {
        callback(MessageType::VERBOSE, message);
    }
}

void SUCCESS(const std::string& message) {
    for (const auto& [_key, callback] : _callbacks) {
        callback(MessageType::SUCCESS, message);
    }
}

void subscribe(void* subscriberAddress, const Callback& callback) {
    _callbacks[subscriberAddress] = callback;
}

void unsubscribe(void* subscriberAddress) {
    _callbacks.erase(subscriberAddress);
}

}  // namespace shkyera::Logger
