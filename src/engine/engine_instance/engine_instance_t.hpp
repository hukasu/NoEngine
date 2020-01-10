/**
 * Engine
 * © Lucas "Hukasu" Farias
 */

#ifndef __ENGINE__INSTANCE__T__HPP__
#define __ENGINE__INSTANCE__T__HPP__

#if defined(_WIN64)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.hpp>

#include <vector>
#include <set>
#include <mutex>

namespace engine {
    class Logger;
    struct EngineInstanceCreateInfo;

    class EngineInstance_T {
        vk::Instance m_vulkan_instance;
        Logger* m_logger;

        uint32_t enumerateVulkanInstanceVersion();
        vk::Instance createVulkanInstance(EngineInstanceCreateInfo* _engine_instance_create_info);
        std::vector<const char*> collectVulkanLayers();
        std::vector<const char*> collectVulkanInstanceExtensions();

        vk::DebugUtilsMessengerCreateInfoEXT prepareDebugUtilsMessenger(
            const std::vector<const char*>& _layers,
            const std::vector<const char*>& _extensions
        );
    public:
        EngineInstance_T(EngineInstanceCreateInfo* _engine_instance_create_info);
        ~EngineInstance_T();

    public:
        static std::set<EngineInstance_T*> pointer_storage;
        static std::mutex pointer_storage_mutex;
        static bool validPointer(EngineInstance_T* _engine_instance_t);
    };
}

#endif // __ENGINE__INSTANCE__T__HPP__