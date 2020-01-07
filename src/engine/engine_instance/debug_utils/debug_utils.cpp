#include "debug_utils.hpp"

#include <engine/engine.hpp>
#include <sstream>

namespace engine {
	LoggerLevel toLoggerLevel(VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity) {
		switch (_message_severity) {
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			return LoggerLevel::eTrace;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			return LoggerLevel::eInformation;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			return LoggerLevel::eWarning;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			return LoggerLevel::eError;
		}
	}

	std::string buildDebugUtilsMessageTypes(VkDebugUtilsMessageTypeFlagsEXT _message_types) {
		std::stringstream message_builder;
		if (_message_types & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
			message_builder << "(General)";
		if (_message_types & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
			message_builder << "(Performance)";
		if (_message_types & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
			message_builder << "(Validation)";
		return message_builder.str();
	}

	VkBool32 debugUtilsMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
		VkDebugUtilsMessageTypeFlagsEXT _message_types,
		const VkDebugUtilsMessengerCallbackDataEXT* _callback_data,
		void* _user_data
	) {
		Logger* logger = static_cast<Logger*>(_user_data);
		std::stringstream message_builder;
		message_builder
			<< buildDebugUtilsMessageTypes(_message_types)
			<< " "
			<< _callback_data->pMessage;
		logger->log(toLoggerLevel(_message_severity), message_builder.str());
		return VK_FALSE;
	}
}