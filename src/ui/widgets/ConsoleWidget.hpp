#pragma once

#include <memory>
#include <vector>

#include "ui/Widget.hpp"

namespace shkyera {

class Log {
  public:
    Log(std::string content);

    virtual void draw() const;
    virtual uint64_t getIconId() const = 0;

    std::string _content;
};

class LogVerbose : public Log {
  public:
    using Log::Log;

    virtual uint64_t getIconId() const override;
};

class LogInfo : public Log {
  public:
    using Log::Log;

    virtual uint64_t getIconId() const override;
};

class LogSuccess : public Log {
  public:
    using Log::Log;

    virtual uint64_t getIconId() const override;
};

class LogError : public Log {
  public:
    using Log::Log;

    virtual uint64_t getIconId() const override;
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

  private:
    void drawInfoBar() const;

    static std::vector<std::shared_ptr<Log>> _logs;
    static bool _verbose;

    static size_t _totalVerbose;
    static size_t _totalInfo;
    static size_t _totalSuccess;
    static size_t _totalError;
};

} // namespace shkyera
