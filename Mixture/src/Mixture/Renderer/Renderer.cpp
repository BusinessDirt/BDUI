#include "mxpch.hpp"
#include "Mixture/Renderer/Renderer.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"

namespace Mixture
{
    Vulkan::Context& Renderer::s_VulkanContext = Vulkan::Context::Get();

    Scope<ShapeRenderer> Renderer::s_ShapeRenderer = CreateScope<ShapeRenderer>();
    Scope<ImGuiRenderer> Renderer::s_ImGuiRenderer = CreateScope<ImGuiRenderer>();
#ifndef OPAL_DIST
    Scope<ImGuiViewport> Renderer::s_ImGuiViewport = CreateScope<ImGuiViewport>();
#endif

    void Renderer::Init(const std::string& applicationName)
    {
        s_VulkanContext.Initialize(applicationName);
        
        s_ShapeRenderer->Initialize();
        s_ImGuiRenderer->Initialize();
#ifndef OPAL_DIST
        s_ImGuiViewport->Initialize();
#endif
    }

    void Renderer::Shutdown()
    {
#ifndef OPAL_DIST
        s_ImGuiViewport->Shutdown();
#endif
        s_ImGuiRenderer->Shutdown();
        s_ShapeRenderer->Shutdown();
        
        s_VulkanContext.Shutdown();
    }

    void Renderer::OnFramebufferResize(uint32_t width, uint32_t height)
    {
        s_VulkanContext.OnFramebufferResize(width, height);
        
        s_ImGuiRenderer->OnFramebufferResize(width, height);
#ifndef OPAL_DIST
        s_ImGuiViewport->OnFramebufferResize();
#endif
    }

    void Renderer::DrawFrame(FrameInfo& frameInfo, const LayerStack& layerStack)
    {
        std::vector<VkCommandBuffer> commandBuffers{};
        
#ifndef OPAL_DIST
        s_ImGuiViewport->Resize();
#endif
        
        if (VkCommandBuffer commandBuffer = s_VulkanContext.BeginFrame())
        {
            s_ShapeRenderer->UploadBuffers();

            s_VulkanContext.BeginRenderpass(commandBuffer);
            
            s_ShapeRenderer->Render(commandBuffer);
            layerStack.Render(frameInfo);
            
            s_VulkanContext.EndRenderpass(commandBuffer);
            
#ifndef OPAL_DIST
            s_ImGuiViewport->BlitFromScene(commandBuffer, s_VulkanContext.Swapchain().GetFramebuffer(s_VulkanContext.CurrentImageIndex()).GetImage(),
                                           s_VulkanContext.Swapchain().GetWidth(), s_VulkanContext.Swapchain().GetHeight());
#endif
            
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

    bool Renderer::DrawImGuiViewport()
    {
#ifndef OPAL_DIST
        return s_ImGuiViewport->DrawWindow();
#else
        return true;
#endif
    }
}
