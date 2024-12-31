#include "mxpch.hpp"
#include "Platform/Vulkan/Swapchain.hpp"

#include "Platform/Vulkan/PhysicalDevice.hpp"

#include "Mixture/Core/Application.hpp"

#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

namespace Vulkan
{
    Swapchain::Swapchain(const PhysicalDevice& physicalDevice, const VkDevice device, const VkSurfaceKHR surface)
        : m_PhysicalDevice(physicalDevice), m_Device(device), m_Surface(surface)
    {
        SwapchainSupportDetails details = m_PhysicalDevice.GetSwapchainSupportDetails();
        ChooseSurfaceFormat(details.Formats);
        ChoosePresentMode(details.PresentModes);
        ChooseExtent(details.Capabilities);
        
        m_ImageCount = details.Capabilities.minImageCount + 1;
        if (details.Capabilities.maxImageCount > 0 && m_ImageCount > details.Capabilities.maxImageCount)
            m_ImageCount = details.Capabilities.maxImageCount;
        
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_Surface;
        createInfo.minImageCount = m_ImageCount;
        createInfo.imageFormat = m_SurfaceFormat.format;
        createInfo.imageColorSpace = m_SurfaceFormat.colorSpace;
        createInfo.imageExtent = m_Extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        
        QueueFamilyIndices indices = physicalDevice.GetQueueFamilyIndices();
        uint32_t queueFamilyIndices[] = { indices.Graphics.value(), indices.Present.value() };

        if (indices.Graphics != indices.Present)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } 
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        
        createInfo.preTransform = details.Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = m_PresentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
        
        VK_ASSERT(vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_Swapchain), "Failed to create Swapchain!");
        
        VULKAN_INFO_BEGIN("Swapchain Details");
        OPAL_CORE_INFO(" - Surface Format: {}, {}", ToString::Format(m_SurfaceFormat.format), ToString::ColorSpace(m_SurfaceFormat.colorSpace));
        OPAL_CORE_INFO(" - Present Mode: {}", ToString::PresentMode(m_PresentMode));
        OPAL_CORE_INFO(" - Extent: {}x{}", m_Extent.width, m_Extent.height);
        VULKAN_INFO_END();
    }

    Swapchain::~Swapchain()
    {
        if (m_Swapchain)
        {
            vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
            m_Swapchain = VK_NULL_HANDLE;
        }
    }

    void Swapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats) 
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                m_SurfaceFormat = availableFormat;
                return;
            }
        }

        m_SurfaceFormat = availableFormats[0];
    }

    void Swapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) 
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                m_PresentMode = availablePresentMode;
                return;
            }
        }

        m_PresentMode = VK_PRESENT_MODE_FIFO_KHR;
    }

    void Swapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
        {
            m_Extent = capabilities.currentExtent;
            return;
        }
        else
        {
            int width, height;
            Mixture::Application::Get().GetWindow().GetFramebufferSize(&width, &height);
            

            VkExtent2D actualExtent = 
            {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            m_Extent = actualExtent;
        }
    }
}
