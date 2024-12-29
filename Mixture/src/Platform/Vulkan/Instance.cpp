#include "mxpch.hpp"
#include "Platform/Vulkan/Instance.hpp"

#include "Platform/Vulkan/DebugMessenger.hpp"

#include <Opal/Base.hpp>

namespace Vulkan
{
    namespace Util
    {
        static std::vector<VkLayerProperties> GetAvailableLayers()
        {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
            
            return availableLayers;
        }

        static std::vector<VkExtensionProperties> GetAvailableExtensions()
        {
            uint32_t extensionCount;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
            
            return availableExtensions;
        }
    
        static bool IsRequired(const char* name, const std::vector<const char*>& required)
        {
            for (const auto& prop : required)
            {
                if (strcmp(name, prop) == 0) return true;
            }
            
            return false;
        }
    }
    
    Instance::Instance(const std::string& applicationName, const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions)
    {
        std::vector<VkLayerProperties> availableLayers = Util::GetAvailableLayers();
        std::vector<VkExtensionProperties> availableExtensions = Util::GetAvailableExtensions();
        
        VULKAN_INFO_BEGIN("Layer Properties");
        for (const auto& layer : availableLayers)
        {
            // Print with the appropriate color: Green if required, Yellow if not required
            OPAL_CORE_INFO("{}{}{}", Util::IsRequired(layer.layerName, requiredLayers) ? AVAILABLE_AND_REQUIRED : AVAILABLE_NOT_REQUIRED,
                layer.layerName, ANSI_RESET);
        }

        // Check for required layer that are not available
        for (const auto& requiredLayer : requiredLayers)
        {
            bool isAvailable = std::any_of(availableLayers.begin(), availableLayers.end(),
                [&requiredLayer](const VkLayerProperties& layer)
                {
                    return strcmp(layer.layerName, requiredLayer) == 0;
                });

            // Red for required but not available
            if (!isAvailable) OPAL_CORE_INFO("{}{}{}", NOT_AVAILABLE_REQUIRED, requiredLayer, ANSI_RESET);
        }
        VULKAN_INFO_END();
        
        VULKAN_INFO_BEGIN("Instance Extension Properties");
        for (const auto& extension : availableExtensions)
        {
            // Print with the appropriate color: Green if required, Yellow if not required
            OPAL_CORE_INFO("{}{}{}", Util::IsRequired(extension.extensionName, requiredExtensions) ? AVAILABLE_AND_REQUIRED : AVAILABLE_NOT_REQUIRED,
                extension.extensionName, ANSI_RESET);
        }

        // Check for required extensions that are not available
        for (const auto& requiredExtension : requiredExtensions)
        {
            bool isAvailable = std::any_of(availableExtensions.begin(), availableExtensions.end(),
                [&requiredExtension](const VkExtensionProperties& ext) 
                {
                    return strcmp(ext.extensionName, requiredExtension) == 0;
                });

            // Red for required but not available
            if (!isAvailable) OPAL_CORE_INFO("{}{}{}", NOT_AVAILABLE_REQUIRED, requiredExtension, ANSI_RESET);
        }
        
        VULKAN_INFO_END();
        
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Mixture";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;
        
//        VkLayerSettingsCreateInfoEXT layerSettingsCreateInfo{};
//        layerSettingsCreateInfo.sType = VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT;
//        layerSettingsCreateInfo.settingCount = static_cast<uint32_t>(manager.GetLayerSettings().size());
//        layerSettingsCreateInfo.pSettings = manager.GetLayerSettings().data();
//        layerSettingsCreateInfo.pNext = nullptr;
        
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
        DebugMessenger::PopulateCreateInfo(debugMessengerCreateInfo);
//        debugMessengerCreateInfo.pNext = &layerSettingsCreateInfo;

        // Instance Creation Info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
#ifdef OPAL_PLATFORM_DARWIN
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR; // required for MoltenVK to work
#endif
        createInfo.pNext = &debugMessengerCreateInfo;

        // Create Vulkan Instance
        VK_ASSERT(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create Vulkan instance!");
    }

    Instance::~Instance()
    {
        if (m_Instance)
        {
            vkDestroyInstance(m_Instance, nullptr);
            m_Instance = VK_NULL_HANDLE;
        }
    }
}
