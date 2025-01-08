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
        VULKAN_INFO_LIST("Surface Format: {}, {}", 0, ToString::Format(m_SurfaceFormat.format), ToString::ColorSpace(m_SurfaceFormat.colorSpace));
        VULKAN_INFO_LIST("Present Mode: {}", 0, ToString::PresentMode(m_PresentMode));
        VULKAN_INFO_LIST("Extent: {} x {}", 0, m_Extent.width, m_Extent.height);
        VULKAN_INFO_END();
        
        // Images
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &m_ImageCount, nullptr);
        m_Images.resize(m_ImageCount);
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &m_ImageCount, m_Images.data());
        
        // Image Views
        CreateImageViews();
    }

    Swapchain::~Swapchain()
    {
        for (auto imageView : m_ImageViews)
        {
            vkDestroyImageView(m_Device, imageView, nullptr);
        }
        
        if (m_Swapchain)
        {
            vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
            m_Swapchain = VK_NULL_HANDLE;
        }
    }

    void Swapchain::CreateImageViews()
    {
        m_ImageViews.resize(m_Images.size());
        
        for (size_t i = 0; i < m_ImageViews.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_Images[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_SurfaceFormat.format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            
            VK_ASSERT(vkCreateImageView(m_Device, &createInfo, nullptr, &m_ImageViews[i]), "Failed to create ImageView!");
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
