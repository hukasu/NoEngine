cmake_minimum_required(VERSION 3.8.2)

project("NoEngine")

find_package(Vulkan REQUIRED)

option(BUILD_EXAMPLES "Build examples" OFF)

add_library(
	engine
	src/engine/engine.cpp
	src/engine/engine_instance/engine_instance.cpp
	src/engine/engine_instance/engine_instance_t.cpp
	src/engine/engine_instance/engine_instance_create_info.cpp
	src/engine/logger/logger.cpp
)
target_include_directories(
	engine
	PUBLIC "include/"
)
target_link_libraries(
	engine
	PUBLIC Vulkan::Vulkan
)
target_compile_features(
	engine
	PUBLIC cxx_std_17
)

if (BUILD_EXAMPLES)
	add_executable(
		cube
		examples/cube/cube.cpp
	)
	target_link_libraries(
		cube
		engine
	)
endif()