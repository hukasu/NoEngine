#include "engine_instance_t.hpp"

#include <engine/engine_instance/engine_instance.hpp>
#include <sstream>
#include <iostream>

#include "debug_utils/debug_utils.hpp"

#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 0
#define ENGINE_VERSION_PATCH 0
#define ENGINE_VERSION VK_MAKE_VERSION(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH)

#define VK_LAYER_STANDARD_VALIDATION_NAME "VK_LAYER_LUNARG_standard_validation"

namespace engine {
	EngineInstance_T::EngineInstance_T(EngineInstanceCreateInfo* _engine_instance_create_info) {
		m_logger = _engine_instance_create_info->m_logger;
		if (m_logger) m_logger->log(LoggerLevel::eTrace, "EngineInstance constructor");
		if (m_logger && m_logger->canLogLevel(LoggerLevel::eInformation)) {
			std::stringstream message_builder;
			message_builder
				<< "Engine:" << std::endl
				<< "\tNoEngine (" << ENGINE_VERSION_MAJOR << "." << ENGINE_VERSION_MINOR << "." << ENGINE_VERSION_PATCH << ")" << std::endl
				<< "Application:" << std::endl
				<< "\t" << _engine_instance_create_info->m_application_name
				<< " ("
				<< VK_VERSION_MAJOR(_engine_instance_create_info->m_application_version)
				<< "."
				<< VK_VERSION_MINOR(_engine_instance_create_info->m_application_version)
				<< "."
				<< VK_VERSION_PATCH(_engine_instance_create_info->m_application_version)
				<< ")";
			m_logger->log(LoggerLevel::eInformation, message_builder.str());
		}

		m_platform_specific_fields.initialize();
		if (m_logger) m_logger->log(LoggerLevel::eTrace, "Initialized platform dependent values");

		m_vulkan_instance = createVulkanInstance(
			_engine_instance_create_info
		);
		if (m_logger) m_logger->log(LoggerLevel::eTrace, "Vulkan Instance created");

		std::unique_lock<std::mutex> lck(pointer_storage_mutex);
		pointer_storage.insert(this);
	}

	EngineInstance_T::~EngineInstance_T() {
		if (m_logger)m_logger->log(LoggerLevel::eTrace, "EngineInstance destructor");
		std::unique_lock<std::mutex> lck(pointer_storage_mutex);
		pointer_storage.erase(pointer_storage.find(this));
		lck.unlock();

		m_vulkan_instance.destroy();
		if (m_logger) m_logger->log(LoggerLevel::eTrace, "Vulkan Instance destroyed");

		m_platform_specific_fields.destroy();
		if (m_logger) m_logger->log(LoggerLevel::eTrace, "Initialized platform dependent values");
	}

	uint32_t EngineInstance_T::enumerateVulkanInstanceVersion() {
		if (m_logger) m_logger->log(LoggerLevel::eTrace, "EngineInstance enumerateVulkanInstanceVersion");
		PFN_vkEnumerateInstanceVersion pfn_instance_version = reinterpret_cast<PFN_vkEnumerateInstanceVersion>(
			vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion")
		);
		if (pfn_instance_version == nullptr) return VK_API_VERSION_1_0;
		else {
			uint32_t version;
			pfn_instance_version(&version);
			return version;
		}
	}

	vk::Instance EngineInstance_T::createVulkanInstance(EngineInstanceCreateInfo* _engine_instance_create_info) {
		if (m_logger) m_logger->log(LoggerLevel::eTrace, "EngineInstance createVulkanInstance");

		vk::ApplicationInfo app_info = {
			_engine_instance_create_info->m_application_name.c_str(),
			_engine_instance_create_info->m_application_version,
			"NoEngine",
			VK_MAKE_VERSION(0,0,0),
			enumerateVulkanInstanceVersion()
		};

		std::vector<const char*> layers = collectVulkanLayers();
		std::vector<const char*> extensions = collectVulkanInstanceExtensions();

		vk::DebugUtilsMessengerCreateInfoEXT debug_messenger = prepareDebugUtilsMessenger(layers, extensions);

		vk::InstanceCreateInfo instance_info = {
			{},
			&app_info,
			static_cast<uint32_t>(layers.size()),
			layers.data(),
			static_cast<uint32_t>(extensions.size()),
			extensions.data()
		};
		if (debug_messenger.pfnUserCallback) {
			instance_info.pNext = &debug_messenger;
		}
		return vk::createInstance(instance_info);
	}

	std::vector<const char*> EngineInstance_T::collectVulkanLayers() {
		if (!m_logger) return {};
		m_logger->log(LoggerLevel::eTrace, "Collecting Vulkan layers");

		std::vector<vk::LayerProperties> vk_layers = vk::enumerateInstanceLayerProperties();
		std::vector<const char*> layers;

		std::set<std::string> vk_layers_set;
		for (auto vk_e : vk_layers) {
			vk_layers_set.insert(std::string(vk_e.layerName));
		}

		/**
		* VK_LAYER_LUNARG_standard_validation
		* OS : All
		* Required : Yes
		* Instance Versions : >1.0
		*/
		if (vk_layers_set.count(VK_LAYER_STANDARD_VALIDATION_NAME))
			layers.push_back(VK_LAYER_STANDARD_VALIDATION_NAME);
		else
			m_logger->log(LoggerLevel::eWarning, "Vulkan Standard Validation layer not available");

		return std::move(layers);
	}

	std::vector<const char*> EngineInstance_T::collectVulkanInstanceExtensions() {
		if (m_logger)
			m_logger->log(LoggerLevel::eTrace, "Collecting Vulkan instance extensions");

		std::vector<vk::ExtensionProperties> vk_extensions = vk::enumerateInstanceExtensionProperties();
		std::vector<const char*> extensions;

		std::set<std::string> vk_exts;
		for (auto vk_e : vk_extensions) {
			vk_exts.insert(std::string(vk_e.extensionName));
		}

		/**
		* VK_KHR_surface
		* OS : All
		* Required : Yes
		* Instance Versions : >1.0
		*/
		if (vk_exts.count(VK_KHR_SURFACE_EXTENSION_NAME))
			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		else
			throw std::runtime_error("TODO Extension not present. (VK_KHR_SURFACE)");
#if defined(_WIN64)
		/**
		* VK_KHR_win32_surface
		* OS : Windows
		* Required : Yes
		* Instance Versions : >1.0
		*/
		if (vk_exts.count(VK_KHR_WIN32_SURFACE_EXTENSION_NAME))
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		else
			throw std::runtime_error("TODO Extension not present. (VK_KHR_WIN32_SURFACE)");
#endif
		/**
		* VK_KHR_get_physical_device_properties2
		* OS : ALL
		* Required : No
		* Instance Versions : 1.0
		*/
		if (vk_exts.count(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
			if (enumerateVulkanInstanceVersion() == VK_API_VERSION_1_0)
				extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		/**
		* VK_KHR_get_surface_capabilities2
		* OS : ALL
		* Required : No
		* Instance Versions : >1.0
		*/
		if (vk_exts.count(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME))
			extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		/**
		* VK_KHR_device_group_creation
		* OS : ALL
		* Required : No
		* Instance Versions : 1.0
		*/
		if (vk_exts.count(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME))
			if (enumerateVulkanInstanceVersion() == VK_API_VERSION_1_0)
				extensions.push_back(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME);
		/**
		* VK_EXT_debug_utils
		* OS : ALL
		* Required : No
		* Instance Versions : >1.0
		*/
		if (m_logger && vk_exts.count(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) 
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return std::move(extensions);
	}

	vk::DebugUtilsMessengerCreateInfoEXT EngineInstance_T::prepareDebugUtilsMessenger(
		const std::vector<const char*>& _layers,
		const std::vector<const char*>& _extensions
	) {
		if (m_logger) {
			m_logger->log(LoggerLevel::eTrace, "Preparing Debug Utils Messenger create info struct.");

			vk::DebugUtilsMessageSeverityFlagsEXT severity;
			switch (m_logger->getLoggerLevel()) {
			case LoggerLevel::eTrace:
				severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;
			case LoggerLevel::eInformation:
				severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;
			case LoggerLevel::eWarning:
				severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
			default:
				severity |= vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
			}
			vk::DebugUtilsMessageTypeFlagsEXT message_types =
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
			vk::DebugUtilsMessengerCreateInfoEXT debug_messenger(
				{},
				severity,
				message_types,
				debugUtilsMessageCallback,
				m_logger
			);
			return debug_messenger;
		}
		return vk::DebugUtilsMessengerCreateInfoEXT();
	}

	std::set<EngineInstance_T*> EngineInstance_T::pointer_storage;
	std::mutex EngineInstance_T::pointer_storage_mutex;

	bool EngineInstance_T::validPointer(EngineInstance_T* _engine_instance_t) {
		std::unique_lock<std::mutex> lck(pointer_storage_mutex);
		auto itr = pointer_storage.find(_engine_instance_t);
		if (pointer_storage.end() == itr) return false;
		else return true;
	}
}