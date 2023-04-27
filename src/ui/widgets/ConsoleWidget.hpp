#pragma once

#include <memory>
#include <vector>

#include "ui/Widget.hpp"

namespace shkyera {

using Log = Widget;

class LogVerbose : public Log {
  public:
    using Log::Log;

    virtual void draw() override;
};

class LogInfo : public Log {
  public:
    using Log::Log;

    virtual void draw() override;
};

class LogSuccess : public Log {
  public:
    using Log::Log;

    virtual void draw() override;
};

class LogError : public Log {
  public:
    using Log::Log;

    virtual void draw() override;
};

class LogPython : public Log {
  public:
    using Log::Log;

    virtual void draw() override;
};

class ConsoleWidget : public Widget {
  public:
    using Widget::Widget;

    virtual void draw() override;

    static void clear();

    static void enableVerbose();
    static void disableVerbose();

    static void logVerbose(std::string text);
    static void logInfo(std::string text);
    static void logSuccess(std::string text);
    static void logError(std::string text);
    static void logPython(std::string text);

  private:
    static std::vector<std::unique_ptr<Log>> _logs;
    static bool _verbose;
};

} // namespace shkyera
