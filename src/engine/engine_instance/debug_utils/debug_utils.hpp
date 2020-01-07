/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __DEBUG__UTILS__HPP__
#define __DEBUG__UTILS__HPP__

#if defined(_WIN64)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.hpp>

namespace engine {
	VkBool32 debugUtilsMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
}

#endif // __DEBUG__UTILS__HPP__