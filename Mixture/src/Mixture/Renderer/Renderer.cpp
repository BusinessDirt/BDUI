#include "mxpch.hpp"
#include "Mixture/Renderer/Renderer.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"

namespace Mixture
{
    Vulkan::Context& Renderer::s_VulkanContext = Vulkan::Context::Get();

    Scope<ShapeRenderer> Renderer::s_ShapeRenderer = CreateScope<ShapeRenderer>();
    Scope<ImGuiRenderer> Renderer::s_ImGuiRenderer = CreateScope<ImGuiRenderer>();

    void Renderer::Init(const std::string& applicationName)
    {
        s_VulkanContext.Initialize(applicationName);
        s_ImGuiRenderer->Initialize();
    }

    void Renderer::Shutdown()
    {
        s_ImGuiRenderer->Shutdown();
        s_VulkanContext.Shutdown();
    }

    void Renderer::OnFramebufferResize(uint32_t width, uint32_t height)
    {
        s_VulkanContext.OnFramebufferResize(width, height);
        
        s_ImGuiRenderer->OnFramebufferResize(width, height);
    }

    void Renderer::DrawFrame(FrameInfo& frameInfo, const LayerStack& layerStack)
    {
        std::vector<VkCommandBuffer> commandBuffers{};

        if (VkCommandBuffer commandBuffer = s_VulkanContext.BeginFrame())
        {
            frameInfo.CommandBuffer = commandBuffer;
            
            s_VulkanContext.BeginRenderpass(commandBuffer);
            layerStack.Render(frameInfo);
            s_VulkanContext.EndRenderpass(commandBuffer);
            
            s_ImGuiRenderer->BeginFrame();
            layerStack.RenderImGui(frameInfo);
            s_ImGuiRenderer->EndFrame();
            
            s_ImGuiRenderer->BeginRenderpass(commandBuffer);
            s_ImGuiRenderer->Draw(commandBuffer);
            s_ImGuiRenderer->EndRenderpass(commandBuffer);
            
            s_VulkanContext.EndFrame(commandBuffer);
            commandBuffers.push_back(commandBuffer);
        }

        s_VulkanContext.SubmitFrame(commandBuffers);
        s_VulkanContext.WaitForDevice();
    }
}
