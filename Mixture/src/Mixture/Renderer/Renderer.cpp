#include "mxpch.hpp"
#include "Mixture/Renderer/Renderer.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"

namespace Mixture
{
    Vulkan::Context& Renderer::s_VulkanContext = Vulkan::Context::Get();
    Scope<LayerStack> Renderer::s_LayerStack = CreateScope<LayerStack>();

    Scope<ImGuiRenderer> Renderer::s_ImGuiRenderer = CreateScope<ImGuiRenderer>();

    void Renderer::Init(const std::string& applicationName)
    {
        s_VulkanContext.Initialize(applicationName);
        s_ImGuiRenderer->Initialize();
    }

    void Renderer::Shutdown()
    {
        s_ImGuiRenderer->Shutdown();
        s_LayerStack = nullptr;
        s_VulkanContext.Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        s_VulkanContext.OnWindowResize(width, height);
        s_ImGuiRenderer->OnWindowResize(width, height);
    }

    void Renderer::DrawFrame()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        startTime = currentTime;

        std::vector<VkCommandBuffer> commandBuffers{};

        if (VkCommandBuffer commandBuffer = s_VulkanContext.BeginFrame())
        {
            FrameInfo frameInfo{};
            frameInfo.FrameIndex = s_VulkanContext.Swapchain().GetCurrentFrameIndex();
            frameInfo.FrameTime = frameTime;
            frameInfo.CommandBuffer = commandBuffer;
            
            s_ImGuiRenderer->BeginFrame();
            s_LayerStack->OnRenderUI(frameInfo);
            s_ImGuiRenderer->EndFrame();

            s_VulkanContext.BeginRenderpass(commandBuffer);
            s_LayerStack->OnUpdate(frameInfo);
            s_VulkanContext.EndRenderpass(commandBuffer);
            
            s_ImGuiRenderer->BeginRenderpass(commandBuffer);
            s_ImGuiRenderer->Draw(commandBuffer);
            s_ImGuiRenderer->EndRenderpass(commandBuffer);
            
            s_VulkanContext.EndFrame(commandBuffer);
            commandBuffers.push_back(commandBuffer);
        }

        s_VulkanContext.SubmitFrame(commandBuffers);
        s_VulkanContext.WaitForDevice();
    }

    void Renderer::OnEvent(Event& e)
    {
        s_LayerStack->OnEvent(e);
    }
}
