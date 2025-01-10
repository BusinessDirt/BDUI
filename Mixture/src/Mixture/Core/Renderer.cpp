#include "mxpch.hpp"
#include "Mixture/Core/Renderer.hpp"

namespace Mixture
{
    Vulkan::Context& Renderer::s_VulkanContext = Vulkan::Context::Get();

    Scope<Vulkan::GraphicsPipeline> Renderer::s_GraphicsPipeline = nullptr;
    Scope<Vulkan::IndexBuffer> Renderer::s_IndexBuffer = nullptr;
    Scope<Vulkan::VertexBuffer> Renderer::s_VertexBuffer = nullptr;

    void Renderer::Init(const std::string& applicationName)
    {
        s_VulkanContext.Initialize(applicationName);

        s_GraphicsPipeline = CreateScope<Vulkan::GraphicsPipeline>();

        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
        };

        const std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        s_VertexBuffer = CreateScope<Vulkan::VertexBuffer>(vertices);
        s_IndexBuffer = CreateScope<Vulkan::IndexBuffer>(indices);
    }

    void Renderer::Shutdown()
    {
        s_IndexBuffer = nullptr;
        s_VertexBuffer = nullptr;
        s_GraphicsPipeline = nullptr;
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

            s_GraphicsPipeline->Bind(frameInfo);
            s_VertexBuffer->Bind(commandBuffer);
            s_IndexBuffer->Bind(commandBuffer);
            vkCmdDrawIndexed(commandBuffer, s_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);

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