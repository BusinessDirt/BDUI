#include "mxpch.hpp"
#include "Platform/Vulkan/Device.hpp"

#include "Platform/Vulkan/PhysicalDevice.hpp"
#include "Platform/Vulkan/Context.hpp"

#include <set>

namespace Mixture::Vulkan
{
    Device::Device(const std::vector<const char*>& requiredLayers, const std::vector<const char*>& requiredExtensions)
    {
        const auto& [Graphics, Present] = Context::Get().PhysicalDevice().GetQueueFamilyIndices();
        
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set uniqueQueueFamilies = { Graphics.value(), Present.value() };

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
        
        VK_ASSERT(vkCreateDevice(Context::Get().PhysicalDevice().GetHandle(), &createInfo, nullptr, &m_Device), "Failed to create logical device!")
        
        // retrieve queues
        vkGetDeviceQueue(m_Device, Graphics.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, Present.value(), 0, &m_PresentQueue);
    }

    Device::~Device()
    {
        if (m_Device)
        {
            vkDestroyDevice(m_Device, nullptr);
            m_Device = VK_NULL_HANDLE;
        }
    }

    VkFormat Device::FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features)
    {
        for (const VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(Context::Get().PhysicalDevice().GetHandle(), format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }

            if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }
        OPAL_CORE_ERROR("Failed to find supported format!");
        return VK_FORMAT_UNDEFINED;
    }

    uint32_t Device::FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(Context::Get().PhysicalDevice().GetHandle(), &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if (typeFilter & 1 << i && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        OPAL_CORE_ERROR("Failed to find suitable memory type!");
        return 0;
    }

    void Device::CreateImageWithInfo(const VkImageCreateInfo& imageInfo, const VkMemoryPropertyFlags properties,
        VkImage& image, VkDeviceMemory& imageMemory) const
    {
        VK_ASSERT(vkCreateImage(m_Device, &imageInfo, nullptr, &image), "Failed to create VkImage!")

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_Device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

        VK_ASSERT(vkAllocateMemory(m_Device, &allocInfo, nullptr, &imageMemory), "Failed to allocate VkImageMemory!")
        VK_ASSERT(vkBindImageMemory(m_Device, image, imageMemory, 0), "Failed to bind VkImageMemory!")
    }
}
