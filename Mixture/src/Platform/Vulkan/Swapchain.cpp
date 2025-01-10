#include "mxpch.hpp"
#include "Platform/Vulkan/Swapchain.hpp"

#include "Platform/Vulkan/Context.hpp"

#include "Mixture/Core/Application.hpp"

#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

namespace Vulkan
{
    Swapchain::Swapchain()
        : m_OldSwapchain(nullptr)
    {
        Init(true);
    }

    Swapchain::Swapchain(std::shared_ptr<Swapchain> previous)
        : m_OldSwapchain(previous)
    {
        Init(false);
        m_OldSwapchain = nullptr;
    }

    void Swapchain::Init(bool debug)
    {
        SwapchainSupportDetails details = Context::Get().m_PhysicalDevice->QuerySwapchainSupport();
        VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(details.Formats);
        VkPresentModeKHR presentMode = ChoosePresentMode(details.PresentModes);
        VkExtent2D extent = ChooseExtent(details.Capabilities);

        uint32_t imageCount = details.Capabilities.minImageCount + 1;
        if (details.Capabilities.maxImageCount > 0 && imageCount > details.Capabilities.maxImageCount)
            imageCount = details.Capabilities.maxImageCount;

        // Swapchain
        {
            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = Context::Get().m_WindowSurface->GetHandle();
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            QueueFamilyIndices indices = Context::Get().m_PhysicalDevice->GetQueueFamilyIndices();
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
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;

            createInfo.oldSwapchain = m_OldSwapchain ? m_OldSwapchain->m_Swapchain : VK_NULL_HANDLE;

            VK_ASSERT(vkCreateSwapchainKHR(Context::Get().m_Device->GetHandle(), &createInfo, nullptr, &m_Swapchain), "Failed to create Swapchain!");

            if (debug)
            {
                VULKAN_INFO_BEGIN("Swapchain Details");
                VULKAN_INFO_LIST("Surface Format: {}, {}", 0, ToString::Format(surfaceFormat.format), ToString::ColorSpace(surfaceFormat.colorSpace));
                VULKAN_INFO_LIST("Present Mode: {}", 0, ToString::PresentMode(presentMode));
                VULKAN_INFO_LIST("Extent: {} x {}", 0, extent.width, extent.height);
                VULKAN_INFO_END();
            } 
        }

        // we only specified a minimum number of images in the swap chain, so the implementation is
        // allowed to create a swap chain with more. That's why we'll first query the final number of
        // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
        // retrieve the handles.
        vkGetSwapchainImagesKHR(Context::Get().m_Device->GetHandle(), m_Swapchain, &imageCount, nullptr);
        std::vector<VkImage> images(imageCount);
        vkGetSwapchainImagesKHR(Context::Get().m_Device->GetHandle(), m_Swapchain, &imageCount, images.data());

        m_Extent = extent;
        m_Renderpass = std::make_unique<Renderpass>(surfaceFormat.format);

        // Depth and Frame buffers
        m_DepthBuffers.resize(imageCount);
        m_FrameBuffers.resize(imageCount);
        for (size_t i = 0; i < imageCount; i++)
        {
            m_DepthBuffers[i] = std::make_unique<DepthBuffer>(extent);
            m_FrameBuffers[i] = std::make_unique<FrameBuffer>(m_DepthBuffers[i]->GetImageView().GetHandle(), images[i],
                extent, surfaceFormat.format, m_Renderpass->GetHandle());
        }

        // sync objects
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(imageCount, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) m_InFlightFences.emplace_back(true);
    }

    Swapchain::~Swapchain()
    {
        for (auto& fb : m_FrameBuffers) fb = nullptr;
        for (auto& db : m_DepthBuffers) db = nullptr;

        if (m_Swapchain)
        {
            vkDestroySwapchainKHR(Context::Get().m_Device->GetHandle(), m_Swapchain, nullptr);
            m_Swapchain = VK_NULL_HANDLE;
        }

        m_Renderpass.reset();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            m_ImageAvailableSemaphores[i].~Semaphore();
            m_RenderFinishedSemaphores[i].~Semaphore();
            m_InFlightFences[i].~Fence();
            m_ImagesInFlight[i] = nullptr;
        }
    }

    VkResult Swapchain::AcquireNextImage()
    {
        m_InFlightFences[m_CurrentFrame].Wait(std::numeric_limits<uint64_t>::max());
        
        return vkAcquireNextImageKHR(Context::Get().m_Device->GetHandle(), m_Swapchain, std::numeric_limits<uint64_t>::max(),
            m_ImageAvailableSemaphores[m_CurrentFrame].GetHandle(), VK_NULL_HANDLE, &Context::Get().m_CurrentImageIndex);
    }

    VkResult Swapchain::SubmitCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers)
    {
        uint32_t currentImageIndex = Context::Get().m_CurrentImageIndex;
        if (m_ImagesInFlight[currentImageIndex] != nullptr)
        {
            m_ImagesInFlight[currentImageIndex]->Wait(std::numeric_limits<uint64_t>::max());
        }
        m_ImagesInFlight[currentImageIndex] = &m_InFlightFences[m_CurrentFrame];

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame].GetHandle() };
        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame].GetHandle() };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        submitInfo.pCommandBuffers = commandBuffers.data();

        m_InFlightFences[m_CurrentFrame].Reset();
        VK_ASSERT(vkQueueSubmit(Context::Get().m_Device->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame].GetHandle()), 
            "Failed to submit draw command buffer!");

        VkSwapchainKHR swapChains[] = { m_Swapchain };
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &Context::Get().m_CurrentImageIndex;

        VkResult result = vkQueuePresentKHR(Context::Get().m_Device->GetPresentQueue(), &presentInfo);
        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        return result;
    }

    bool Swapchain::CompareSwapFormats(const Swapchain& swapChain) const
    {
        return GetImageFormat() == swapChain.GetImageFormat() && GetDepthFormat() == swapChain.GetDepthFormat();
    }

    VkSurfaceFormatKHR Swapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats) 
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR Swapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes) 
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) return capabilities.currentExtent;
        
        VkExtent2D actualExtent = { Mixture::Application::Get().GetWindow().GetWidth(), Mixture::Application::Get().GetWindow().GetHeight() };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
