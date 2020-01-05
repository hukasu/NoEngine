/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __ENGINE__INSTANCE__HPP__
#define __ENGINE__INSTANCE__HPP__

#include "../logger/logger.hpp"
#include <string>

namespace engine {
	class EngineInstance_T;

	/// <summary>Structure used in the creation of EngineInstances</summary>
	struct EngineInstanceCreateInfo {
		/// <summary>Application;s name</summary>
		std::string m_application_name;
		/// <summary>Application's Version</summary>
		uint32_t m_application_version;
		/// <summary>Engine's Logger</summary>
		Logger* m_logger;

		/// <summary>Creates and EngineInstanceCreateInfo object</summary>
		/// <param name='_application_name>Application's name</param>
		/// <param name='_application_name>Application's version</param>
		/// <param name='_logger>Engine's logger, setting this parameter implicitly enables logging of engine operations</param>
		EngineInstanceCreateInfo(
			std::string _application_name = "",
			uint32_t _application_version = 0,
			Logger* _logger = nullptr
		);

		/// <summary>Chain setter for Application Name</summary>
		/// <param name='_application_name>Application's name</param>
		EngineInstanceCreateInfo& setApplicationName(std::string _application_name);
		/// <summary>Chain setter for Application Version</summary>
		/// <param name='_application_name>Application's version</param>
		EngineInstanceCreateInfo& setApplicationVersion(uint32_t _application_version);
		/// <summary>Chain setter for Logger</summary>
		/// <param name='_application_name>Engine's logger, setting this parameter implicitly enables logging of engine operations</param>
		EngineInstanceCreateInfo& setLogger(Logger* _logger);
	};

	/// <summary>Engine's starting point</summary>
	class EngineInstance {
		/// <summary>Pointer to internal objects</summary>
		EngineInstance_T* m_engine_instance_t;
	public:
		/// <summary>Empty constructor</summary>
		EngineInstance();
		/// <summary>Null constructor</summary>
		EngineInstance(std::nullptr_t);
		/// <summary>Complete constructor</summary>
		EngineInstance(EngineInstance_T* _engine_instance);

		/// <summary>Verifies if EngineInstances are equal</summary>
		/// <param name='_rhs'>The other EngineInstance</param>
		bool operator==(EngineInstance const& _rhs) const;
		/// <summary>Verifies if current EngineInstance is smaller than <paramref name='_rhs' /></summary>
		/// <param name='_rhs'>The other EngineInstance</param>
		bool operator<(EngineInstance const& _rhs) const;
		/// <summary>Verifies if current EngineInstance is valid</summary>
		operator bool() const;
		/// <summary>Cast to internal object</summary>
		operator EngineInstance_T* () const;

		/// <summary>Destroys EngineInstance</summary>
		/// <remarks>Any undestroyed child object will be destroyed</remarks>
		void destroy();
	};


}

#endif // __ENGINE__INSTANCE__HPP__