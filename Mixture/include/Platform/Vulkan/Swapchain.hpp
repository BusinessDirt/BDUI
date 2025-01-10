#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Renderpass.hpp"

#include "Platform/Vulkan/Command/Buffers.hpp"

#include "Platform/Vulkan/Buffer/Frame.hpp"
#include "Platform/Vulkan/Buffer/Depth.hpp"

#include "Platform/Vulkan/Sync/Fence.hpp"
#include "Platform/Vulkan/Sync/Semaphore.hpp"

namespace Mixture::Vulkan
{
    class Swapchain
    {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        Swapchain();
        Swapchain(std::shared_ptr<Swapchain> previous);
        ~Swapchain();

        VkExtent2D GetExtent() const { return m_Extent; }
        uint32_t GetWidth() const { return m_Extent.width; }
        uint32_t GetHeight() const { return m_Extent.height; }

        const VkFormat GetImageFormat() const { return m_FrameBuffers[0]->GetFormat(); }
        const VkFormat GetDepthFormat() const { return m_DepthBuffers[0]->GetFormat(); }

        const Renderpass& GetRenderpass() const { return *m_Renderpass; }
        const FrameBuffer& GetFramebuffer(int index) const { return *m_FrameBuffers[index]; }

        uint32_t GetCurrentFrameIndex() const { return static_cast<uint32_t>(m_CurrentFrame); }
        size_t GetImageCount() const { return m_FrameBuffers.size(); }

        VkResult AcquireNextImage();
        VkResult SubmitCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers);

        bool CompareSwapFormats(const Swapchain& swapChain) const;
        
    private:
        void Init(bool debug);
        VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR  ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        
    private:
        VULKAN_HANDLE(VkSwapchainKHR, m_Swapchain);
        std::shared_ptr<Swapchain> m_OldSwapchain;
        
        VkExtent2D m_Extent;
        size_t m_CurrentFrame = 0;

        Scope<Renderpass> m_Renderpass = nullptr;
        std::vector<Scope<FrameBuffer>> m_FrameBuffers;
        std::vector<Scope<DepthBuffer>> m_DepthBuffers;

        std::vector<Semaphore> m_ImageAvailableSemaphores;
        std::vector<Semaphore> m_RenderFinishedSemaphores;
        std::vector<Fence> m_InFlightFences;
        std::vector<Fence*> m_ImagesInFlight;
    };
}
