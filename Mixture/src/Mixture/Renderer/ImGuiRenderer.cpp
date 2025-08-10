#include "mxpch.hpp"
#include "Mixture/Renderer/ImGuiRenderer.hpp"

#include "Mixture/Core/Application.hpp"
#include "Mixture/Util/ToString.hpp"

#include "Platform/Vulkan/Context.hpp"
#include "Platform/Vulkan/Command/SingleTime.hpp"

#include <imgui.h>
#include <backends/imgui_impl_vulkan.cpp>
#include <backends/imgui_impl_glfw.cpp>

namespace Mixture
{
    ImGuiRenderer::~ImGuiRenderer()
    {
        Shutdown();
    }

    void ImGuiRenderer::Initialize()
    {
        const Vulkan::Swapchain& swapchain = Vulkan::Context::Get().Swapchain();
        m_Width = swapchain.GetWidth();
        m_Height = swapchain.GetHeight();
        
        // 1. Create Descriptor Pool for ImGui
        {
            VkDescriptorPoolSize poolSizes[] =
            {
                { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
            };
            
            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
            poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
            poolInfo.pPoolSizes = poolSizes;
            VK_ASSERT(vkCreateDescriptorPool(Vulkan::Context::Get().Device().GetHandle(), &poolInfo, nullptr, &m_DescriptorPool),
                      "Failed to create Descriptor Pool")
        }
        
        CreateRenderpass(swapchain);
        CreateFramebuffers(swapchain);
        
        // ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        
        ImGui::StyleColorsDark();

        // Init ImGui Vulkan backend with the VkRenderPass handle
        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.ApiVersion = VK_API_VERSION_1_2;
        init_info.Instance = Vulkan::Context::Get().Instance().GetHandle();
        init_info.PhysicalDevice = Vulkan::Context::Get().PhysicalDevice().GetHandle();
        init_info.Device = Vulkan::Context::Get().Device().GetHandle();
        init_info.QueueFamily = Vulkan::Context::Get().PhysicalDevice().GetQueueFamilyIndices().Graphics.value();
        init_info.Queue = Vulkan::Context::Get().Device().GetGraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = m_DescriptorPool;
        init_info.RenderPass = m_Renderpass->GetHandle();
        init_info.Subpass = 0;
        init_info.MinImageCount = Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT;
        init_info.ImageCount = (uint32_t)swapchain.GetImageCount();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = [](VkResult err) {
            if (err != VK_SUCCESS)
                throw std::runtime_error("ImGui Vulkan backend error");
        };

        ImGui_ImplVulkan_Init(&init_info);
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow(), true);
    }

    void ImGuiRenderer::Shutdown()
    {
        if (!m_Renderpass && !m_DescriptorPool) return;
        
        Vulkan::Context::Get().WaitForDevice();
        
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        m_FrameBuffers.clear();
        m_Renderpass.reset();
        
        if (m_DescriptorPool != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(Vulkan::Context::Get().Device().GetHandle(), m_DescriptorPool, nullptr);
            m_DescriptorPool = VK_NULL_HANDLE;
        }
    }

    void ImGuiRenderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        Vulkan::Context::Get().WaitForDevice();
        
        m_FrameBuffers.clear();
        const Vulkan::Swapchain& swapchain = Vulkan::Context::Get().Swapchain();
        CreateFramebuffers(swapchain);
    }

    void ImGuiRenderer::BeginFrame()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)m_Width, (float)m_Height);
        
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::EndFrame()
    {
        ImGui::Render();
    }

    void ImGuiRenderer::BeginRenderpass(VkCommandBuffer commandBuffer)
    {
        VkRenderPassBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        beginInfo.renderPass = m_Renderpass->GetHandle();
        beginInfo.framebuffer = m_FrameBuffers[Vulkan::Context::Get().CurrentImageIndex()]->GetHandle();
        beginInfo.renderArea.offset = {0, 0};
        beginInfo.renderArea.extent.width = m_Width;
        beginInfo.renderArea.extent.height = m_Height;
        beginInfo.clearValueCount = 0;
        beginInfo.pClearValues = nullptr;

        vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void ImGuiRenderer::Draw(VkCommandBuffer commandBuffer)
    {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    void ImGuiRenderer::EndRenderpass(VkCommandBuffer commandBuffer)
    {
        vkCmdEndRenderPass(commandBuffer);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiRenderer::CreateFramebuffers(const Vulkan::Swapchain& swapchain)
    {
        m_FrameBuffers.resize(swapchain.GetImageCount());
        for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
        {
            const Vulkan::FrameBuffer& fb = swapchain.GetFramebuffer(i);
            m_FrameBuffers[i] = CreateScope<Vulkan::FrameBuffer>(VK_NULL_HANDLE, fb.GetImage(), swapchain.GetExtent(), fb.GetFormat(), m_Renderpass->GetHandle());
        }
    }

    void ImGuiRenderer::CreateRenderpass(const Vulkan::Swapchain& swapchain)
    {
        const VkFormat format = swapchain.GetImageFormat();
        m_Renderpass = CreateScope<Vulkan::Renderpass>(format, false,
            VK_ATTACHMENT_LOAD_OP_LOAD, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    }
}
