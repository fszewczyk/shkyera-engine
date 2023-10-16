/**
 * @file ConsoleWidget.hpp
 *
 * @brief Contains the declaration of classes related to logging and a console user interface widget.
 */

#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "ui/Widget.hpp"

namespace shkyera {

/**
 * @brief A base class for logs that contain textual content.
 */
class Log {
  public:
    /**
     * @brief Constructor to create a log with the specified content.
     *
     * @param content The text content of the log.
     */
    Log(std::string content);

    /**
     * @brief Virtual method to draw the log in the user interface.
     */
    virtual void draw() const;

    /**
     * @brief Virtual method to retrieve an icon identifier associated with the log.
     *
     * @return An icon identifier.
     */
    virtual uint64_t getIconId() const = 0;

    std::string _content; ///< The text content of the log.
};

/**
 * @brief A log representing verbose messages.
 */
class LogVerbose : public Log {
  public:
    using Log::Log;

    /**
     * @brief Get the icon identifier for verbose logs.
     *
     * @return An icon identifier for verbose logs.
     */
    virtual uint64_t getIconId() const override;
};

/**
 * @brief A log representing informational messages.
 */
class LogInfo : public Log {
  public:
    using Log::Log;

    /**
     * @brief Get the icon identifier for informational logs.
     *
     * @return An icon identifier for informational logs.
     */
    virtual uint64_t getIconId() const override;
};

/**
 * @brief A log representing success messages.
 */
class LogSuccess : public Log {
  public:
    using Log::Log;

    /**
     * @brief Get the icon identifier for success logs.
     *
     * @return An icon identifier for success logs.
     */
    virtual uint64_t getIconId() const override;
};

/**
 * @brief A log representing error messages.
 */
class LogError : public Log {
  public:
    using Log::Log;

    /**
     * @brief Get the icon identifier for error logs.
     *
     * @return An icon identifier for error logs.
     */
    virtual uint64_t getIconId() const override;
};

/**
 * @brief A user interface widget for the console, used for logging and displaying messages.
 */
class ConsoleWidget : public Widget {
  public:
    using Widget::Widget;

    /**
     * @brief Implementation of the abstract `draw` method to render the console widget.
     */
    virtual void draw() override;

    /**
     * @brief Clear all logs from the console widget.
     */
    static void clear();

    /**
     * @brief Enable verbose logging mode.
     */
    static void enableVerbose();

    /**
     * @brief Disable verbose logging mode.
     */
    static void disableVerbose();

    /**
     * @brief Log a verbose message to the console.
     *
     * @param text The verbose message to log.
     */
    static void logVerbose(std::string text);

    /**
     * @brief Log an informational message to the console.
     *
     * @param text The informational message to log.
     */
    static void logInfo(std::string text);

    /**
     * @brief Log a success message to the console.
     *
     * @param text The success message to log.
     */
    static void logSuccess(std::string text);

    /**
     * @brief Log an error message to the console.
     *
     * @param text The error message to log.
     */
    static void logError(std::string text);

    /**
     * @brief The maximum number of logs to keep in the console.
     */
    static constexpr size_t MAX_LOGS = 50;

  private:
    /**
     * @brief Draw the information bar of the console widget.
     */
    void drawInfoBar() const;

    static size_t _totalVerbose; ///< The total number of verbose logs.
    static size_t _totalInfo;    ///< The total number of informational logs.
    static size_t _totalSuccess; ///< The total number of success logs.
    static size_t _totalError;   ///< The total number of error logs.

    static std::vector<std::shared_ptr<Log>> _logs; ///< A collection of logs in the console.
    static bool _verbose;                           ///< A flag indicating verbose logging mode.
};

} // namespace shkyera
