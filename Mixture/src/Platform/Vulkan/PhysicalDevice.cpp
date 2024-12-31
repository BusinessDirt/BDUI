#include "mxpch.hpp"
#include "Platform/Vulkan/PhysicalDevice.hpp"

#include <Opal/Base.hpp>

#include <set>

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

        
    }

    PhysicalDevice::PhysicalDevice(VkInstance instance, const VkSurfaceKHR surface, const std::vector<const char*>& requiredExtensions)
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
            int score = RateDeviceSuitability(device, requiredExtensions);
            candidates.insert(std::make_pair(score, device));
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0) 
        {
            m_PhysicalDevice = candidates.rbegin()->second;
            m_QueueFamilyIndices = FindQueueFamilyIndices(m_PhysicalDevice);
            m_SwapchainSupportDetails = QuerySwapchainSupport(m_PhysicalDevice);
            
            Util::PrintDebugAvailability(Util::GetAvailableExtensions(m_PhysicalDevice), requiredExtensions, [](const VkExtensionProperties& extension) { return extension.extensionName; }, "Device Extensions");
        }
        
        OPAL_CORE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find suitable GPU!");
        
        // Debug Information
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
        vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);

        VULKAN_INFO_BEGIN("Selected GPU Information");
        OPAL_CORE_INFO(" - Device Name: {}", m_Properties.deviceName);
        OPAL_CORE_INFO(" - Device Type: {}", ToString::PhysicalDeviceType(m_Properties.deviceType));
        OPAL_CORE_INFO(" - API Version: {}", ToString::Version(m_Properties.apiVersion));
        OPAL_CORE_INFO(" - Driver Version: {}", ToString::Version(m_Properties.driverVersion));
        OPAL_CORE_INFO(" - Vendor ID: 0x{:X}", m_Properties.vendorID);
        OPAL_CORE_INFO(" - Device ID: 0x{:X}", m_Properties.deviceID);
        OPAL_CORE_INFO(" - Limits:");
        OPAL_CORE_INFO(" -   Max Image Dimension 2D: {}", m_Properties.limits.maxImageDimension2D);
        OPAL_CORE_INFO(" -   Max Uniform Buffer Range: {}", m_Properties.limits.maxUniformBufferRange);
        OPAL_CORE_INFO(" -   Max Viewports: {}", m_Properties.limits.maxViewports);
        VULKAN_INFO_END();

    }

    int PhysicalDevice::RateDeviceSuitability(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions)
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
        
        // Device Extensions need to be supported
        if (!CheckExtensionSupport(device, requiredExtensions)) score = 0;
        
        // Swapchain support (has to have atleast one format and present mode)
        SwapchainSupportDetails swapchainSupport = QuerySwapchainSupport(device);
        if (swapchainSupport.Formats.empty() || swapchainSupport.PresentModes.empty()) score = 0;

        return score;
    }

    bool PhysicalDevice::CheckExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> extensionSet(requiredExtensions.begin(), requiredExtensions.end());

        for (const auto& extension : availableExtensions)
        {
            extensionSet.erase(extension.extensionName);
        }

        return extensionSet.empty();
    }

    QueueFamilyIndices PhysicalDevice::FindQueueFamilyIndices(const VkPhysicalDevice device)
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

    SwapchainSupportDetails PhysicalDevice::QuerySwapchainSupport(const VkPhysicalDevice device)
    {
        SwapchainSupportDetails details;
        
        // Basic surface capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.Capabilities);
        
        // Supported surface formats
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

        if (formatCount != 0) 
        {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.Formats.data());
        }
        
        // Supported presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) 
        {
            details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.PresentModes.data());
        }
        
        return details;
    }
}
