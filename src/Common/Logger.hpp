#pragma once

#include <mutex>
#include <queue>

namespace shkyera::Logger {

enum class MessageType { ERROR, INFO, VERBOSE, SUCCESS };

using Callback = std::function<void(MessageType messageType, const std::string& message)>;

void ERROR(const std::string& message);
void INFO(const std::string& message);
void VERBOSE(const std::string& message);
void SUCCESS(const std::string& message);

void subscribe(void* subscriberAddress, const Callback& callback);
void unsubscribe(void* subscriberAddress);

}  // namespace shkyera::Logger
