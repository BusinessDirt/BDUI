#include "mxpch.hpp"
#include "Platform/Vulkan/Device.hpp"

#include "Platform/Vulkan/PhysicalDevice.hpp"

#include <set>

namespace Vulkan
{
    Device::Device(const PhysicalDevice& physicalDevice, const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions)
        : m_PhysicalDevice(physicalDevice)
    {
        const QueueFamilyIndices& indices = physicalDevice.GetQueueFamilyIndices();
        
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.Graphics.value(), indices.Present.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) 
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }
        
        VkPhysicalDeviceFeatures deviceFeatures{};
        
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        
        VK_ASSERT(vkCreateDevice(m_PhysicalDevice.GetHandle(), &createInfo, nullptr, &m_Device), "Failed to create logical device!");
        
        // retrieve queues
        vkGetDeviceQueue(m_Device, indices.Graphics.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, indices.Present.value(), 0, &m_PresentQueue);
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
