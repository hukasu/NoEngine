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

#define logTrace(x1, x2) x1.log(engine::LoggerLevel::eTrace, x2, std::string(__FILE__), __LINE__)
#define logInfo(x1, x2) x1.log(engine::LoggerLevel::eInformation, x2, std::string(__FILE__), __LINE__)
#define logWarning(x1, x2) x1.log(engine::LoggerLevel::eWarning, x2, std::string(__FILE__), __LINE__)
#define logError(x1, x2) x1.log(engine::LoggerLevel::eError, x2, std::string(__FILE__), __LINE__)

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

        void log(MessageSeverity _message_severity, std::string _message, std::string _file, uint32_t _line);
    };
}

#endif // __LOGGER__HPP__