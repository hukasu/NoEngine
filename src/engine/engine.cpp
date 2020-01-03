#include <engine/engine.hpp>

#include <stdexcept>
#include "engine_instance/engine_instance_t.hpp"

namespace engine {
	uint32_t makeVersionNumber(uint16_t _major, uint16_t _minor, uint16_t _patch) {
		if (_major > 0x3ff || _minor > 0x3ff || _patch > 0xfff) throw std::runtime_error("TODO");
		return VK_MAKE_VERSION(_major, _minor, _patch);
	}

	EngineInstance createEngineInstance(EngineInstanceCreateInfo* _engine_instance_create_info) {
		return EngineInstance(new EngineInstance_T(_engine_instance_create_info));
	}
}