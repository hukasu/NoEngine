#include <engine/engine_instance/engine_instance.hpp>

#include "engine_instance_t.hpp"

namespace engine {
	EngineInstance::EngineInstance() {
		m_engine_instance_t = nullptr;
	}

	EngineInstance::EngineInstance(std::nullptr_t) {
		m_engine_instance_t = nullptr;
	}

	EngineInstance::EngineInstance(EngineInstance_T* _engine_instance_t) {
		m_engine_instance_t = _engine_instance_t;
	}

	bool EngineInstance::operator==(EngineInstance const& _rhs) const {
		return m_engine_instance_t == _rhs.m_engine_instance_t;
	}

	bool EngineInstance::operator<(EngineInstance const& _rhs) const {
		return m_engine_instance_t < _rhs.m_engine_instance_t;
	}

	EngineInstance::operator bool() const {
		return m_engine_instance_t;
	}

	EngineInstance::operator EngineInstance_T* () const {
		return m_engine_instance_t;
	}

	void EngineInstance::destroy() {
		if (EngineInstance_T::validPointer(m_engine_instance_t)) delete m_engine_instance_t;
		else throw std::runtime_error("TODO");
	}
}