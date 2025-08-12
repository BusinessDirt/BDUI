#include "mxpch.hpp"
#include "Mixture/Renderer/Renderer.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"

namespace Mixture
{
    Vulkan::Context& Renderer::s_VulkanContext = Vulkan::Context::Get();
    Scope<LayerStack> Renderer::s_LayerStack = CreateScope<LayerStack>();

    Scope<ImGuiRenderer> Renderer::s_ImGuiRenderer = CreateScope<ImGuiRenderer>();
#ifndef OPAL_DIST
    Scope<ImGuiViewport> Renderer::s_ImGuiViewport = CreateScope<ImGuiViewport>();
#endif

    void Renderer::Init(const std::string& applicationName)
    {
        s_VulkanContext.Initialize(applicationName);
        s_ImGuiRenderer->Initialize();
#ifndef OPAL_DIST
        s_ImGuiViewport->Initialize();
#endif
    }

    void Renderer::Shutdown()
    {
        s_LayerStack = nullptr;
        
#ifndef OPAL_DIST
        s_ImGuiViewport->Shutdown();
#endif
        s_ImGuiRenderer->Shutdown();
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

    void Renderer::DrawFrame()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        startTime = currentTime;

        std::vector<VkCommandBuffer> commandBuffers{};
        
#ifndef OPAL_DIST
        s_ImGuiViewport->Resize();
#endif
        
        if (VkCommandBuffer commandBuffer = s_VulkanContext.BeginFrame())
        {
            FrameInfo frameInfo{};
            frameInfo.FrameIndex = s_VulkanContext.Swapchain().GetCurrentFrameIndex();
            frameInfo.FrameTime = frameTime;
            frameInfo.CommandBuffer = commandBuffer;

            s_VulkanContext.BeginRenderpass(commandBuffer);
            s_LayerStack->OnUpdate(frameInfo);
            s_VulkanContext.EndRenderpass(commandBuffer);
            
#ifndef OPAL_DIST
            s_ImGuiViewport->BlitFromScene(commandBuffer, s_VulkanContext.Swapchain().GetFramebuffer(s_VulkanContext.CurrentImageIndex()).GetImage(),
                                           s_VulkanContext.Swapchain().GetWidth(), s_VulkanContext.Swapchain().GetHeight());
#endif
            
            s_ImGuiRenderer->BeginFrame();
            s_LayerStack->OnRenderUI(frameInfo);
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

    void Renderer::DrawImGuiViewport()
    {
#ifndef OPAL_DIST
            s_ImGuiViewport->DrawWindow();
#endif
    }

    void Renderer::OnEvent(Event& e)
    {
        s_LayerStack->OnEvent(e);
    }
}
