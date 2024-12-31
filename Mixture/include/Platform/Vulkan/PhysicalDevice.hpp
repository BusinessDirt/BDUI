#pragma once
#include "Platform/Vulkan/Base.hpp"

#include "Platform/Vulkan/WindowSurface.hpp"

#include <Opal/Base.hpp>

namespace Vulkan
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> Graphics;
        std::optional<uint32_t> Present;
        
        bool IsComplete()
        {
            return Graphics.has_value() && Present.has_value();
        }
    };

    class PhysicalDevice
    {
    public:
        PhysicalDevice(const VkInstance instance, const VkSurfaceKHR surface);
        ~PhysicalDevice() = default;
        
        OPAL_NON_COPIABLE(PhysicalDevice);
        
        const VkPhysicalDeviceProperties& GetProperties() const { return m_Properties; }
        const VkPhysicalDeviceFeatures& GetFeatures() const { return m_Features; }
        const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }
        
    private:
        int RateDeviceSuitability(VkPhysicalDevice device);
        QueueFamilyIndices FindQueueFamilyIndices(VkPhysicalDevice device);
        
    private:
        VULKAN_HANDLE(VkPhysicalDevice, m_PhysicalDevice);
        
        VkPhysicalDeviceProperties m_Properties;
        VkPhysicalDeviceFeatures m_Features;
        QueueFamilyIndices m_QueueFamilyIndices;
        
        const VkInstance m_Instance;
        const VkSurfaceKHR m_Surface;
    };
}
