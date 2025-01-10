#include "mxpch.hpp"
#include "Platform/Vulkan/Context.hpp"

#include <Opal/Base.hpp>

#ifdef OPAL_PLATFORM_DARWIN
    #include <vulkan/vulkan_metal.h>
#endif

#ifdef OPAL_PLATFORM_WINDOWS
    #include <vulkan/vulkan_win32.h>
#endif

namespace Vulkan
{
    namespace Util
    {
        static std::vector<const char*> GetRequiredLayers()
        {
            std::vector<const char*> requiredLayers;

            // Common layers for validation
        #ifndef OPAL_DIST
            requiredLayers.push_back("VK_LAYER_KHRONOS_validation");
        #endif

            return requiredLayers;
        }

        static std::vector<const char*> GetRequiredInstanceExtensions()
        {
            std::vector<const char*> requiredExtensions;

            // Common extensions
            requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            
        #if defined(OPAL_PLATFORM_WINDOWS)
            requiredExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        #elif defined(OPAL_PLATFORM_DARWIN)
            requiredExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            requiredExtensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
        #elif defined(OPAL_PLATFORM_LINUX)
            requiredExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
        #endif

        #ifndef OPAL_DIST
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #endif

            return requiredExtensions;
        }
    
        static std::vector<const char*> GetRequiredDeviceExtensions()
        {
            std::vector<const char*> requiredExtensions;
            
            // Swapchain
            requiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            
        #ifdef OPAL_PLATFORM_DARWIN
            // Required for MoltenVK
            requiredExtensions.push_back("VK_KHR_portability_subset");
        #endif

            return requiredExtensions;
        }
    }

    std::unique_ptr<Context> Context::s_Instance = nullptr;
    std::mutex Context::s_Mutex;

    Context& Context::Get()
    {
        std::lock_guard<std::mutex> lock(s_Mutex);
        if (s_Instance == nullptr) s_Instance = std::make_unique<Context>();
        return *s_Instance;
    }

    void Context::Initialize(const std::string& applicationName)
    {
        m_Instance.reset(new Instance(applicationName, Util::GetRequiredLayers(), Util::GetRequiredInstanceExtensions()));
        
    #ifndef OPAL_DIST
        m_DebugMessenger.reset(new DebugMessenger());
    #endif
        
        m_WindowSurface.reset(new WindowSurface());
        m_PhysicalDevice.reset(new PhysicalDevice(Util::GetRequiredDeviceExtensions()));
        m_Device.reset(new Device(Util::GetRequiredLayers(), Util::GetRequiredDeviceExtensions()));
        m_CommandPool.reset(new CommandPool());
        m_Swapchain.reset(new Swapchain());
        m_CommandBuffers.reset(new CommandBuffers(m_CommandPool->GetHandle(), Swapchain::MAX_FRAMES_IN_FLIGHT));
        m_GraphicsPipeline.reset(new GraphicsPipeline());
    }

    void Context::Shutdown()
    {
        m_GraphicsPipeline = nullptr;
        m_CommandBuffers = nullptr;
        m_Swapchain = nullptr;
        m_CommandPool = nullptr;
        m_Device = nullptr;
        m_PhysicalDevice = nullptr;
        m_WindowSurface = nullptr;
        m_DebugMessenger = nullptr;
        m_Instance = nullptr;
    }

    void Context::OnWindowResize(uint32_t width, uint32_t height)
    {
        m_WindowResized = true;
    }

    void Context::WaitForDevice()
    {
        vkDeviceWaitIdle(m_Device->GetHandle());
    }

    VkCommandBuffer Context::BeginFrame()
    {
        OPAL_CORE_ASSERT(!m_IsFrameStarted, "Can't call BeginFrame() while already in progess!");

        VkResult result = m_Swapchain->AcquireNextImage();
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RebuildSwapchain();
            return VK_NULL_HANDLE;
        }

        OPAL_CORE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR, "Failed to acquire Swapchain image!");

        m_IsFrameStarted = true;

        VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording command buffer!");

        return commandBuffer;
    }

    void Context::BeginRenderpass(VkCommandBuffer commandBuffer)
    {
        OPAL_CORE_ASSERT(m_IsFrameStarted, "Can't call BeginRenderPass() if frame is not in progress!");

        VkRenderPassBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        beginInfo.renderPass = m_Swapchain->GetRenderpass().GetHandle();
        beginInfo.framebuffer = m_Swapchain->GetFramebuffer(m_CurrentImageIndex).GetHandle();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        beginInfo.renderArea.offset = { 0, 0 };
        beginInfo.renderArea.extent = m_Swapchain->GetExtent();
        beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        beginInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_Swapchain->GetExtent().width);
        viewport.height = static_cast<float>(m_Swapchain->GetExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{ { 0, 0 }, m_Swapchain->GetExtent() };

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void Context::EndRenderpass(VkCommandBuffer commandBuffer)
    {
        OPAL_CORE_ASSERT(m_IsFrameStarted, "Can't call EndRenderPass() if frame is not in progress!");
        OPAL_CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't end render pass on command buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);
    }

    void Context::EndFrame(VkCommandBuffer commandBuffer)
    {
        OPAL_CORE_ASSERT(m_IsFrameStarted, "Can't call EndFrame() while frame is not in progress!");
        VK_ASSERT(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer!");
    }

    void Context::SubmitFrame(const std::vector<VkCommandBuffer>& commandBuffers)
    {
        VkResult result = m_Swapchain->SubmitCommandBuffers(commandBuffers);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_WindowResized)
        {
            RebuildSwapchain();
            m_WindowResized = false;
        }
        else
        {
            OPAL_CORE_ASSERT(result == VK_SUCCESS, "Failed to present Swapchain image!");
        }

        m_IsFrameStarted = false;
    }

    void Context::RebuildSwapchain()
    {
        WaitForDevice();

        std::shared_ptr<Swapchain> oldSwapchain = std::move(Context::Get().m_Swapchain);
        Context::Get().m_Swapchain = std::make_unique<Swapchain>(oldSwapchain);

        OPAL_CORE_ASSERT(oldSwapchain->CompareSwapFormats(*Context::Get().m_Swapchain.get()), "Swap chain image (or depth) format has changed!");
    }
}
