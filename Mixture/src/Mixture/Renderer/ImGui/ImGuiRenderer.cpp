#include "mxpch.hpp"
#include "Mixture/Renderer/ImGui/ImGuiRenderer.hpp"

#include "Mixture/Core/Application.hpp"

#include "Platform/Vulkan/Context.hpp"
#include "Platform/Vulkan/Command/SingleTime.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_vulkan.cpp>

namespace Mixture
{
    ImGuiRenderer::~ImGuiRenderer()
    {
        Shutdown();
    }

    void ImGuiRenderer::Initialize()
    {
        Vulkan::Context& context = Vulkan::Context::Get();
        const Vulkan::Swapchain& swapchain = context.Swapchain();
        m_Width = swapchain.GetWidth();
        m_Height = swapchain.GetHeight();
        
        const VkFormat format = swapchain.GetImageFormat();
        m_Renderpass = CreateScope<Vulkan::Renderpass>(format, false,
            VK_ATTACHMENT_LOAD_OP_LOAD, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        
        CreateFramebuffers();
        
        // ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        
        ImGui::StyleColorsDark();

        // Init ImGui Vulkan backend with the VkRenderPass handle
        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.ApiVersion = VK_API_VERSION_1_2;
        initInfo.Instance = context.Instance().GetHandle();
        initInfo.PhysicalDevice = context.PhysicalDevice().GetHandle();
        initInfo.Device = context.Device().GetHandle();
        initInfo.QueueFamily = context.PhysicalDevice().GetQueueFamilyIndices().Graphics.value();
        initInfo.Queue = context.Device().GetGraphicsQueue();
        initInfo.PipelineCache = VK_NULL_HANDLE;
        initInfo.DescriptorPool = context.DescriptorPool().GetGlobalHandle();
        initInfo.RenderPass = m_Renderpass->GetHandle();
        initInfo.Subpass = 0;
        initInfo.MinImageCount = Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT;
        initInfo.ImageCount = static_cast<uint32_t>(swapchain.GetImageCount());
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = nullptr;
        initInfo.CheckVkResultFn = [](const VkResult res) { OPAL_CORE_ASSERT(res == VK_SUCCESS, "ImGui Vulkan backend error") };

        ImGui_ImplVulkan_Init(&initInfo);
        ImGui_ImplGlfw_InitForVulkan(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), true);
    }

    void ImGuiRenderer::Shutdown()
    {
        if (!m_Renderpass) return;
        
        Vulkan::Context::Get().WaitForDevice();
        
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        m_FrameBuffers.clear();
        m_Renderpass.reset();
    }

    void ImGuiRenderer::OnFramebufferResize(const uint32_t width, const uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        Vulkan::Context::Get().WaitForDevice();
        
        m_FrameBuffers.clear();
        CreateFramebuffers();
    }

    void ImGuiRenderer::BeginFrame() const
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(m_Width), static_cast<float>(m_Height));
        
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::EndFrame()
    {
        ImGui::Render();
    }

    void ImGuiRenderer::BeginRenderpass(const VkCommandBuffer commandBuffer) const
    {
        VkRenderPassBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        beginInfo.renderPass = m_Renderpass->GetHandle();
        beginInfo.framebuffer = m_FrameBuffers[Vulkan::Context::Get().CurrentImageIndex()]->GetHandle();
        beginInfo.renderArea.offset = {.x = 0, .y = 0};
        beginInfo.renderArea.extent.width = m_Width;
        beginInfo.renderArea.extent.height = m_Height;
        beginInfo.clearValueCount = 0;
        beginInfo.pClearValues = nullptr;

        vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void ImGuiRenderer::Draw(const VkCommandBuffer commandBuffer)
    {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    void ImGuiRenderer::EndRenderpass(const VkCommandBuffer commandBuffer)
    {
        vkCmdEndRenderPass(commandBuffer);

        if (const ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiRenderer::CreateFramebuffers()
    {
        const Vulkan::Swapchain& swapchain = Vulkan::Context::Get().Swapchain();
        m_FrameBuffers.resize(swapchain.GetImageCount());
        for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
        {
            const Vulkan::FrameBuffer& fb = swapchain.GetFramebuffer(i);
            m_FrameBuffers[i] = CreateScope<Vulkan::FrameBuffer>(VK_NULL_HANDLE, fb.GetImage(), swapchain.GetExtent(), fb.GetFormat(), m_Renderpass->GetHandle());
        }
    }
}
