#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Renderpass.hpp"

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

        OPAL_NON_COPIABLE(Swapchain);
        
        explicit Swapchain(const std::shared_ptr<Swapchain>& previous);
        Swapchain();
        ~Swapchain();

        OPAL_NODISCARD VkExtent2D GetExtent() const { return m_Extent; }
        OPAL_NODISCARD uint32_t GetWidth() const { return m_Extent.width; }
        OPAL_NODISCARD uint32_t GetHeight() const { return m_Extent.height; }

        OPAL_NODISCARD VkFormat GetImageFormat() const { return m_FrameBuffers[0]->GetFormat(); }
        OPAL_NODISCARD VkFormat GetDepthFormat() const { return m_DepthBuffers[0]->GetFormat(); }

        OPAL_NODISCARD const Renderpass& GetRenderpass() const { return *m_Renderpass; }
        OPAL_NODISCARD const FrameBuffer& GetFramebuffer(const uint32_t index) const { return *m_FrameBuffers[index]; }

        OPAL_NODISCARD uint32_t GetCurrentFrameIndex() const { return static_cast<uint32_t>(m_CurrentFrame); }
        OPAL_NODISCARD size_t GetImageCount() const { return m_FrameBuffers.size(); }

        OPAL_NODISCARD VkResult AcquireNextImage() const;
        OPAL_NODISCARD VkResult SubmitCommandBuffers(const std::vector<VkCommandBuffer>& commandBuffers);

        OPAL_NODISCARD bool CompareSwapFormats(const Swapchain& swapChain) const;
        
    private:
        void Init(bool debug);
        static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR  ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        static VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        
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
