#pragma once

#include "Mixture/Events/ApplicationEvent.hpp"

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Instance.hpp"
#include "Platform/Vulkan/DebugMessenger.hpp"
#include "Platform/Vulkan/WindowSurface.hpp"
#include "Platform/Vulkan/PhysicalDevice.hpp"
#include "Platform/Vulkan/Device.hpp"
#include "Platform/Vulkan/Swapchain.hpp"
#include "Platform/Vulkan/DescriptorPool.hpp"
#include "Platform/Vulkan/Command/Pool.hpp"
#include "Platform/Vulkan/Command/Buffers.hpp"

namespace Mixture::Vulkan
{
    #define VK_CONTEXT_MEMBER(type, varName) private: Scope<type> varName = nullptr; public: const type& type() { return *varName; }

    class Context
    {
    public:
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

        static Context& Get();

        Context() = default;
        ~Context() = default;
        
        void Initialize(const std::string& applicationName);
        void Shutdown();

        void OnWindowResize(uint32_t width, uint32_t height);

        void WaitForDevice();
        VkCommandBuffer BeginFrame();
        void BeginRenderpass(VkCommandBuffer commandBuffer);
        void EndRenderpass(VkCommandBuffer commandBuffer);
        void EndFrame(VkCommandBuffer commandBuffer);
        void SubmitFrame(const std::vector<VkCommandBuffer>& commandBuffers);
        
        uint32_t CurrentImageIndex() { return m_CurrentImageIndex; }

    private:
        VkCommandBuffer GetCurrentCommandBuffer() const
        {
            OPAL_CORE_ASSERT(m_IsFrameStarted, "Cannot get command buffer when frame is not in progress!");
            return m_CommandBuffers->Get(m_Swapchain->GetCurrentFrameIndex());
        }

        void RebuildSwapchain();
        
        VK_CONTEXT_MEMBER(Instance, m_Instance);
        VK_CONTEXT_MEMBER(DebugMessenger, m_DebugMessenger);
        VK_CONTEXT_MEMBER(WindowSurface, m_WindowSurface);
        VK_CONTEXT_MEMBER(PhysicalDevice, m_PhysicalDevice);
        VK_CONTEXT_MEMBER(Device, m_Device);
        VK_CONTEXT_MEMBER(Swapchain, m_Swapchain);
        VK_CONTEXT_MEMBER(CommandPool, m_CommandPool);
        VK_CONTEXT_MEMBER(CommandBuffers, m_CommandBuffers);
        VK_CONTEXT_MEMBER(DescriptorPool, m_DescriptorPool);

        uint32_t m_CurrentImageIndex = 0;

    private:
        bool m_IsFrameStarted = false;
        bool m_WindowResized = false;

    private:
        static Scope<Context> s_Instance;
        static std::mutex s_Mutex;
    };
}
