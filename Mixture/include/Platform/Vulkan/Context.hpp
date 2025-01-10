#pragma once

#include "Mixture/Events/ApplicationEvent.hpp"

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Instance.hpp"
#include "Platform/Vulkan/DebugMessenger.hpp"
#include "Platform/Vulkan/WindowSurface.hpp"
#include "Platform/Vulkan/PhysicalDevice.hpp"
#include "Platform/Vulkan/Device.hpp"
#include "Platform/Vulkan/Swapchain.hpp"
#include "Platform/Vulkan/Command/Pool.hpp"
#include "Platform/Vulkan/Command/Buffers.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

namespace Vulkan
{
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

    private:
        VkCommandBuffer GetCurrentCommandBuffer() const
        {
            OPAL_CORE_ASSERT(m_IsFrameStarted, "Cannot get command buffer when frame is not in progress!");
            return m_CommandBuffers->Get(m_Swapchain->GetCurrentFrameIndex());
        }

        void RebuildSwapchain();
        
    public:
        std::unique_ptr<Instance> m_Instance = nullptr;
        std::unique_ptr<DebugMessenger> m_DebugMessenger = nullptr;
        std::unique_ptr<WindowSurface> m_WindowSurface = nullptr;
        std::unique_ptr<PhysicalDevice> m_PhysicalDevice = nullptr;
        std::unique_ptr<Device> m_Device = nullptr;
        std::unique_ptr<Swapchain> m_Swapchain = nullptr;
        std::unique_ptr<CommandPool> m_CommandPool = nullptr;
        std::unique_ptr<CommandBuffers> m_CommandBuffers = nullptr;
        std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline = nullptr;

        uint32_t m_CurrentImageIndex = 0;

    private:
        bool m_IsFrameStarted = false;
        bool m_WindowResized = false;

    private:
        static std::unique_ptr<Context> s_Instance;
        static std::mutex s_Mutex;
    };
}
