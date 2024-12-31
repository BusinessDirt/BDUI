#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Vulkan
{
    class PhysicalDevice;

    class Swapchain
    {
    public:
        Swapchain(const PhysicalDevice& physicalDevice, const VkDevice device, const VkSurfaceKHR surface);
        ~Swapchain();
        
    private:
        void ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        void ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        void ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        
    private:
        VULKAN_HANDLE(VkSwapchainKHR, m_Swapchain);
        
        VkSurfaceFormatKHR m_SurfaceFormat;
        VkPresentModeKHR m_PresentMode;
        VkExtent2D m_Extent;
        uint32_t m_ImageCount;
        
        const PhysicalDevice& m_PhysicalDevice;
        const VkDevice m_Device;
        const VkSurfaceKHR m_Surface;
    };
}
