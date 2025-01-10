#include "mxpch.hpp"
#include "Mixture/Core/Renderer.hpp"

namespace Mixture
{
    Vulkan::Context& Renderer::s_VulkanContext = Vulkan::Context::Get();

    void Renderer::Init(const std::string& applicationName)
    {
        s_VulkanContext.Initialize(applicationName);
    }

    void Renderer::Shutdown()
    {
        s_VulkanContext.Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        s_VulkanContext.OnWindowResize(width, height);
    }

    void Renderer::DrawFrame()
    {
        std::vector<VkCommandBuffer> commandBuffers{};

        if (VkCommandBuffer commandBuffer = s_VulkanContext.BeginFrame())
        {
            FrameInfo frameInfo{};
            frameInfo.FrameIndex = s_VulkanContext.m_Swapchain->GetCurrentFrameIndex();
            frameInfo.CommandBuffer = commandBuffer;

            s_VulkanContext.BeginRenderpass(commandBuffer);

            s_VulkanContext.m_GraphicsPipeline->Bind(frameInfo);
            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

            s_VulkanContext.EndRenderpass(commandBuffer);
            s_VulkanContext.EndFrame(commandBuffer);
            commandBuffers.push_back(commandBuffer);
        }

        s_VulkanContext.SubmitFrame(commandBuffers);
        s_VulkanContext.WaitForDevice();
    }

    void Renderer::OnEvent(Event& e)
    {
        
    }
}