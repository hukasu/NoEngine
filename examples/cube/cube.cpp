#include <engine/engine.hpp>
#include <iostream>

int main(int argc, char** argv) {
	engine::Logger logger(std::cout, engine::LoggerLevel::eTrace);
	engine::EngineInstanceCreateInfo engine_instance_info = {
		"Cube",
		engine::makeVersionNumber(0, 0, 0),
		&logger
	};
	engine::EngineInstance engine_instance = engine::createEngineInstance(&engine_instance_info);
	engine_instance.destroy();
}