/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __ENGINE__INSTANCE__HPP__
#define __ENGINE__INSTANCE__HPP__

#include <string>

namespace engine {
	class EngineInstance_T;

	// Structure used in the creation of EngineInstances
	struct EngineInstanceCreateInfo {
		// Application name
		std::string m_application_name;
		// Application Version
		uint32_t m_application_version;

		EngineInstanceCreateInfo(
			std::string _application_name = "",
			uint32_t _application_version = 0
		);

		// Chain setter for Application Name
		EngineInstanceCreateInfo& setApplicationName(std::string _application_name);
		// Chain setter for Application Version
		EngineInstanceCreateInfo& setApplicationVersion(uint32_t _application_version);
	};

	class EngineInstance {
		EngineInstance_T* m_engine_instance_t;
	public:
		EngineInstance();
		EngineInstance(std::nullptr_t);
		EngineInstance(EngineInstance_T* _engine_instance);

		bool operator==(EngineInstance const& _rhs) const;
		bool operator<(EngineInstance const& _rhs) const;
		operator bool() const;
		operator EngineInstance_T* () const;

		void destroy();
	};


}

#endif // __ENGINE__INSTANCE__HPP__