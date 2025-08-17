#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Instance.hpp"
#include "Platform/Vulkan/DebugMessenger.hpp"
#include "Platform/Vulkan/WindowSurface.hpp"
#include "Platform/Vulkan/PhysicalDevice.hpp"
#include "Platform/Vulkan/Device.hpp"
#include "Platform/Vulkan/Swapchain.hpp"
#include "Platform/Vulkan/Descriptor/Pool.hpp"
#include "Platform/Vulkan/Command/Pool.hpp"
#include "Platform/Vulkan/Command/Buffers.hpp"

namespace Mixture::Vulkan
{
    #define VK_CONTEXT_MEMBER(type, varName) private: Scope<type> varName = nullptr; public: const type& type() { return *varName; }

    class Context
    {
    public:
        OPAL_NON_COPIABLE(Context);

        static Context& Get();

        Context() = default;
        ~Context() = default;
        
        void Initialize(const std::string& applicationName);
        void Shutdown();

        void OnFramebufferResize(uint32_t width, uint32_t height);

        void WaitForDevice() const;
        VkCommandBuffer BeginFrame();
        void BeginRenderpass(VkCommandBuffer commandBuffer) const;
        void EndRenderpass(VkCommandBuffer commandBuffer) const;
        void EndFrame(VkCommandBuffer commandBuffer) const;
        void SubmitFrame(const std::vector<VkCommandBuffer>& commandBuffers);
        
        OPAL_NODISCARD uint32_t CurrentImageIndex() const { return m_CurrentImageIndex; }
        OPAL_NODISCARD uint32_t* CurrentImageIndexPtr() { return &m_CurrentImageIndex; }

    private:
        OPAL_NODISCARD VkCommandBuffer GetCurrentCommandBuffer() const
        {
            OPAL_CORE_ASSERT(m_IsFrameStarted, "Cannot get command buffer when frame is not in progress!")
            return m_CommandBuffers->Get(m_Swapchain->GetCurrentFrameIndex());
        }

        void RebuildSwapchain();

        uint32_t m_CurrentImageIndex = 0;
        VK_CONTEXT_MEMBER(Instance, m_Instance)
        VK_CONTEXT_MEMBER(DebugMessenger, m_DebugMessenger)
        VK_CONTEXT_MEMBER(WindowSurface, m_WindowSurface)
        VK_CONTEXT_MEMBER(PhysicalDevice, m_PhysicalDevice)
        VK_CONTEXT_MEMBER(Device, m_Device)
        VK_CONTEXT_MEMBER(Swapchain, m_Swapchain)
        VK_CONTEXT_MEMBER(CommandPool, m_CommandPool)
        VK_CONTEXT_MEMBER(CommandBuffers, m_CommandBuffers)
        VK_CONTEXT_MEMBER(DescriptorPool, m_DescriptorPool)

    private:
        bool m_IsFrameStarted = false;
        bool m_FramebufferResized = false;

    private:
        static Scope<Context> s_Instance;
        static std::mutex s_Mutex;
    };
}
