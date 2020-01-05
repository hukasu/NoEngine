#include <engine/logger/logger.hpp>

#include <sstream>

namespace engine {
	std::ostream& operator<<(std::ostream& _os, const LoggerLevel _logger_level) {
		switch (_logger_level) {
		case LoggerLevel::eTrace:
			_os << "[TRACE]";
			break;
		case LoggerLevel::eInformation:
			_os << "[INFO ]";
			break;
		case LoggerLevel::eWarning:
			_os << "[WARN ]";
			break;
		case LoggerLevel::eError:
			_os << "[ERROR]";
			break;
		default:
			_os << "[?????]";
			break;
		}
		return _os;
	}

	Logger::Logger(std::ostream& _output, LoggerLevel _logger_level) : m_output(_output), m_logger_level(_logger_level) {
		m_thread_running = true;
		m_logger_thread = std::thread(&Logger::loggerThread, this);
	}

	Logger::~Logger() {
		m_thread_running = false;
		m_message_queue_cv.notify_one();
		m_logger_thread.join();
	}

	void Logger::loggerThread() {
		while (m_thread_running) {
			std::unique_lock<std::mutex> lck(m_message_queue_mutex);
			m_message_queue_cv.wait(lck);
			while (!m_message_queue.empty()) {
				std::string message = m_message_queue.front();
				m_message_queue.pop();
				m_output << message;
			}
		}
	}

	void Logger::changeLoggerLevel(LoggerLevel _logger_level) {
		m_logger_level = _logger_level;
	}

	LoggerLevel Logger::getLoggerLevel() const {
		return m_logger_level;
	}

	void Logger::log(MessageSeverity _message_severity, std::string _message) {
		if (!m_thread_running || _message_severity < m_logger_level) return;
		std::stringstream message_builder;
		message_builder << _message_severity << ": " << _message;
		std::string message = message_builder.str();
		std::unique_lock lck(m_message_queue_mutex);
		m_message_queue.push(message);
		m_message_queue_cv.notify_one();
	}
}

