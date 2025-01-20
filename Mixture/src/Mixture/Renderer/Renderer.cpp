#include "mxpch.hpp"
#include "Mixture/Renderer/Renderer.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"
#include "Mixture/Renderer/ShapeRenderer.hpp"

namespace Mixture
{
    Vulkan::Context& Renderer::s_VulkanContext = Vulkan::Context::Get();
    Scope<LayerStack> Renderer::s_LayerStack = CreateScope<LayerStack>();

    void Renderer::Init(const std::string& applicationName)
    {
        s_VulkanContext.Initialize(applicationName);
        ShapeRenderer::Initialize();
    }

    void Renderer::Shutdown()
    {
        s_LayerStack = nullptr;
        ShapeRenderer::Shutdown();
        s_VulkanContext.Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        s_VulkanContext.OnWindowResize(width, height);
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
            frameInfo.FrameIndex = s_VulkanContext.m_Swapchain->GetCurrentFrameIndex();
            frameInfo.FrameTime = frameTime;
            frameInfo.CommandBuffer = commandBuffer;

            s_VulkanContext.BeginRenderpass(commandBuffer);

            s_LayerStack->Update(frameInfo);
            ShapeRenderer::Render(frameInfo);

            s_VulkanContext.EndRenderpass(commandBuffer);
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
