#include <engine/engine.hpp>

int main(int argc, char** argv) {
	engine::EngineInstanceCreateInfo engine_instance_info = {
		"Cube",
		engine::makeVersionNumber(0, 0, 0)
	};
	engine::EngineInstance engine_instance = engine::createEngineInstance(&engine_instance_info);
	engine_instance.destroy();
}