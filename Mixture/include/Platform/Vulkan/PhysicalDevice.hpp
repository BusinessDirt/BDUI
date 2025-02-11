#pragma once
#include "Platform/Vulkan/Base.hpp"

#include "Platform/Vulkan/WindowSurface.hpp"

#include <Opal/Base.hpp>

namespace Mixture::Vulkan
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

    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    class PhysicalDevice
    {
    public:
        PhysicalDevice(const std::vector<const char*>& requiredExtensions);
        ~PhysicalDevice() = default;
        
        OPAL_NON_COPIABLE(PhysicalDevice);
        
        const VkPhysicalDeviceProperties& GetProperties() const { return m_Properties; }
        const VkPhysicalDeviceFeatures& GetFeatures() const { return m_Features; }
        const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

        SwapchainSupportDetails QuerySwapchainSupport(const VkPhysicalDevice device = VK_NULL_HANDLE) const;

    private:
        int RateDeviceSuitability(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions);
        bool CheckExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions);
        
        QueueFamilyIndices FindQueueFamilyIndices(const VkPhysicalDevice device);
        
    private:
        VULKAN_HANDLE(VkPhysicalDevice, m_PhysicalDevice);
        
        VkPhysicalDeviceProperties m_Properties;
        VkPhysicalDeviceFeatures m_Features;
        
        QueueFamilyIndices m_QueueFamilyIndices;
    };
}
