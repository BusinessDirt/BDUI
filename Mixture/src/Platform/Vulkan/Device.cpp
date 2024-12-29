#include "mxpch.hpp"
#include "Platform/Vulkan/Device.hpp"

#include "Platform/Vulkan/PhysicalDevice.hpp"

namespace Vulkan
{
    namespace Util
    {
        static std::vector<VkExtensionProperties> GetAvailableExtensions(const VkPhysicalDevice physicalDevice)
        {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());
            
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

    Device::Device(const PhysicalDevice& physicalDevice, const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions)
        : m_PhysicalDevice(physicalDevice)
    {
        // verify Extension availability
        std::vector<VkExtensionProperties> availableExtensions = Util::GetAvailableExtensions(physicalDevice.GetHandle());
        
        VULKAN_INFO_BEGIN("Device Extension Properties");
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
        
        const QueueFamilyIndices& indices = physicalDevice.GetQueueFamilyIndices();
        
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.Graphics.value();
        queueCreateInfo.queueCount = 1;
        
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        
        VkPhysicalDeviceFeatures deviceFeatures{};
        
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        
        VK_ASSERT(vkCreateDevice(m_PhysicalDevice.GetHandle(), &createInfo, nullptr, &m_Device), "Failed to create logical device!");
        
        // retrieve queues
        vkGetDeviceQueue(m_Device, indices.Graphics.value(), 0, &m_GraphicsQueue);
    }

    Device::~Device()
    {
        if (m_Device)
        {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = VK_NULL_HANDLE;
        }
    }
};
