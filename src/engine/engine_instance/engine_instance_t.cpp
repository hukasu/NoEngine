#include "engine_instance_t.hpp"

#include <engine/engine_instance/engine_instance.hpp>
#include <iostream>

namespace engine {
	EngineInstance_T::EngineInstance_T(EngineInstanceCreateInfo* _engine_instance_create_info) {
		m_vulkan_instance = createVulkanInstance(
			_engine_instance_create_info
		);

		std::unique_lock<std::mutex> lck(pointer_storage_mutex);
		pointer_storage.insert(this);
	}

	EngineInstance_T::~EngineInstance_T() {
		std::unique_lock<std::mutex> lck(pointer_storage_mutex);
		pointer_storage.erase(pointer_storage.find(this));
		lck.unlock();

		m_vulkan_instance.destroy();
	}

	uint32_t EngineInstance_T::enumerateVulkanInstanceVersion() {
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
		vk::ApplicationInfo app_info = {
			_engine_instance_create_info->m_application_name.c_str(),
			_engine_instance_create_info->m_application_version,
			"NoEngine",
			VK_MAKE_VERSION(0,0,0),
			enumerateVulkanInstanceVersion()
		};

		std::vector<const char*> layers;
		std::vector<const char*> extensions;

		vk::InstanceCreateInfo instance_info = {
			{},
			&app_info,
			static_cast<uint32_t>(layers.size()),
			layers.data(),
			static_cast<uint32_t>(extensions.size()),
			extensions.data()
		};
		return vk::createInstance(instance_info);
	}

	std::vector<const char*> EngineInstance_T::collectVulkanInstanceExtensions() {
		std::vector<vk::ExtensionProperties> vk_extensions = vk::enumerateInstanceExtensionProperties();
		std::vector<const char*> extensions;

		std::set<std::string> vk_exts;
		for (auto vk_e : vk_extensions) {
			vk_exts.insert(std::string(vk_e.extensionName));
		}

		/**
		* VK_KHR_SURFACE
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
		* VK_KHR_SURFACE
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
		* VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2
		* OS : ALL
		* Required : No
		* Instance Versions : 1.0
		*/
		if (vk_exts.count(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
			if (enumerateVulkanInstanceVersion() == VK_API_VERSION_1_0)
				extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		/**
		* VK_KHR_GET_SURFACE_CAPABILITIES_2
		* OS : ALL
		* Required : No
		* Instance Versions : >1.0
		*/
		if (vk_exts.count(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME))
			extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		/**
		* VK_KHR_DEVICE_GROUP_CREATION
		* OS : ALL
		* Required : No
		* Instance Versions : 1.0
		*/
		if (vk_exts.count(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME))
			if (enumerateVulkanInstanceVersion() == VK_API_VERSION_1_0)
				extensions.push_back(VK_KHR_DEVICE_GROUP_CREATION_EXTENSION_NAME);

		return std::move(extensions);
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