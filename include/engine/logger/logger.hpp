/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __LOGGER__HPP__
#define __LOGGER__HPP__

#include <string>
#include <ostream>
#include <queue>
#include <atomic>
#include <thread>
#include <condition_variable>

namespace engine {
    enum class LoggerLevel {
        eTrace,
        eInformation,
        eWarning,
        eError,
        eVerbose = eTrace
    };
    typedef LoggerLevel MessageSeverity;

    class Logger {
        std::atomic<bool> m_thread_running;
        std::thread m_logger_thread;
        std::ostream& m_output;
        LoggerLevel m_logger_level;
        std::mutex m_message_queue_mutex;
        std::condition_variable m_message_queue_cv;
        std::queue<std::string> m_message_queue;

        void loggerThread();
    public:
        Logger(std::ostream& _output, LoggerLevel _logger_level);
        ~Logger();

        void changeLoggerLevel(LoggerLevel _logger_level);
        LoggerLevel getLoggerLevel() const;
        bool canLogLevel(LoggerLevel _logger_level) const;

        void log(MessageSeverity _message_severity, std::string _message);
    };
}

#endif // __LOGGER__HPP__