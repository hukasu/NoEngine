#include <engine/engine_instance/engine_instance.hpp>

namespace engine {
	EngineInstanceCreateInfo::EngineInstanceCreateInfo(
		std::string _application_name,
		uint32_t _application_version,
		Logger* _logger
	) {
		m_application_name = _application_name;
		m_application_version = _application_version;
		m_logger = _logger;
	}

	EngineInstanceCreateInfo& EngineInstanceCreateInfo::setApplicationName(std::string _application_name) {
		m_application_name = _application_name;
		return *this;
	}

	EngineInstanceCreateInfo& EngineInstanceCreateInfo::setApplicationVersion(uint32_t _application_version) {
		m_application_version = _application_version;
		return *this;
	}

	EngineInstanceCreateInfo& EngineInstanceCreateInfo::setLogger(Logger* _logger) {
		m_logger = _logger;
		return *this;
	}
}