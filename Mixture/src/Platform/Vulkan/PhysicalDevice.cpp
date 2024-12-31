#include "mxpch.hpp"
#include "Platform/Vulkan/PhysicalDevice.hpp"

#include <Opal/Base.hpp>

namespace Vulkan
{
    PhysicalDevice::PhysicalDevice(VkInstance instance, const VkSurfaceKHR surface)
        : m_Instance(instance), m_Surface(surface)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
        
        OPAL_CORE_ASSERT(deviceCount > 0, "Failed to find GPU with Vulkan support!");
        
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
        
        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices) 
        {
            int score = RateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0) 
        {
            m_PhysicalDevice = candidates.rbegin()->second;
            m_QueueFamilyIndices = FindQueueFamilyIndices(m_PhysicalDevice);
        }
        
        OPAL_CORE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find suitable GPU!");
        
        // Debug Information
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
        vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);

        VULKAN_INFO_BEGIN("Selected GPU Information");
        OPAL_CORE_INFO(" - Device Name: {}", m_Properties.deviceName);
        OPAL_CORE_INFO(" - Device Type: {}",
            (m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? "Discrete GPU" :
             m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ? "Integrated GPU" :
             m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU ? "Virtual GPU" :
             m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "CPU" :
             "Other"));
        OPAL_CORE_INFO(" - API Version: {}.{}.{}",
            VK_VERSION_MAJOR(m_Properties.apiVersion),
            VK_VERSION_MINOR(m_Properties.apiVersion),
            VK_VERSION_PATCH(m_Properties.apiVersion));
        OPAL_CORE_INFO(" - Driver Version: {}.{}.{}",
            VK_VERSION_MAJOR(m_Properties.driverVersion),
            VK_VERSION_MINOR(m_Properties.driverVersion),
            VK_VERSION_PATCH(m_Properties.driverVersion));
        OPAL_CORE_INFO(" - Vendor ID: 0x{:X}", m_Properties.vendorID);
        OPAL_CORE_INFO(" - Device ID: 0x{:X}", m_Properties.deviceID);
        OPAL_CORE_INFO(" - Limits:");
        OPAL_CORE_INFO(" -   Max Image Dimension 2D: {}", m_Properties.limits.maxImageDimension2D);
        OPAL_CORE_INFO(" -   Max Uniform Buffer Range: {}", m_Properties.limits.maxUniformBufferRange);
        OPAL_CORE_INFO(" -   Max Viewports: {}", m_Properties.limits.maxViewports);
        VULKAN_INFO_END();

    }

    int PhysicalDevice::RateDeviceSuitability(VkPhysicalDevice device)
    {
        // Basic device properites like name, type, supported vulkan versions, etc
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        
        // Optional features like texture compression, multi viewport rendering, 64 it floats, etc
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
        
        int score = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;
        
        // These features need to be supported for the application to work
        // ===============================================================
        
        // Graphics and Present Queue
        QueueFamilyIndices indices = FindQueueFamilyIndices(device);
        if (!indices.IsComplete()) score = 0;

        return score;
    }

    QueueFamilyIndices PhysicalDevice::FindQueueFamilyIndices(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;
        
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        
        int i = 0;
        for (const auto& queueFamily : queueFamilies) 
        {
            // Graphics Queue
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.Graphics = i;
            
            // Present Queue
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
            if (presentSupport) indices.Present = i;

            // break early if all indices have been found
            if (indices.IsComplete()) break;
            
            i++;
        }
        
        return indices;
    }
}
