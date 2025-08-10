#include "mxpch.hpp"
#include "Mixture/Renderer/ImGuiRenderer.hpp"

#include "Platform/Vulkan/Context.hpp"
#include "Platform/Vulkan/Command/SingleTime.hpp"

#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>

namespace Mixture
{
    ImGuiRenderer::~ImGuiRenderer()
    {
        Shutdown();
    }

    void ImGuiRenderer::Initialize()
    {
        const Vulkan::Swapchain& swapchain = *Vulkan::Context::Get().m_Swapchain;
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
            VK_ASSERT(vkCreateDescriptorPool(Vulkan::Context::Get().m_Device->GetHandle(), &poolInfo, nullptr, &m_DescriptorPool),
                      "Failed to create Descriptor Pool")
        }
        
        const VkFormat format = swapchain.GetImageFormat();
        m_Renderpass = CreateScope<Vulkan::Renderpass>(format, false,
            VK_ATTACHMENT_LOAD_OP_CLEAR, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        
        CreateFramebuffers();
        
        // ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        // Init ImGui Vulkan backend with the VkRenderPass handle
        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = Vulkan::Context::Get().m_Instance->GetHandle();
        init_info.PhysicalDevice = Vulkan::Context::Get().m_PhysicalDevice->GetHandle();
        init_info.Device = Vulkan::Context::Get().m_Device->GetHandle();
        init_info.QueueFamily = Vulkan::Context::Get().m_PhysicalDevice->GetQueueFamilyIndices().Graphics.value();
        init_info.Queue = Vulkan::Context::Get().m_Device->GetGraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = m_DescriptorPool;
        init_info.RenderPass = m_Renderpass->GetHandle();
        init_info.MinImageCount = Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT;
        init_info.ImageCount = (uint32_t)Vulkan::Context::Get().m_Swapchain->GetImageCount();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = [](VkResult err) {
            if (err != VK_SUCCESS)
                throw std::runtime_error("ImGui Vulkan backend error");
        };

        ImGui_ImplVulkan_Init(&init_info);
    }

    void ImGuiRenderer::Shutdown()
    {
        if (!m_Renderpass && !m_DescriptorPool) return;
        
        Vulkan::Context::Get().WaitForDevice();
        
        m_FrameBuffers.clear();
        m_Renderpass.reset();
        
        if (m_DescriptorPool != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(Vulkan::Context::Get().m_Device->GetHandle(), m_DescriptorPool, nullptr);
            m_DescriptorPool = VK_NULL_HANDLE;
        }
        
        ImGui_ImplVulkan_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiRenderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;

        Vulkan::Context::Get().WaitForDevice();
        
        // Recreate framebuffer objects
        m_FrameBuffers.clear();
        CreateFramebuffers();
    }

    void ImGuiRenderer::RenderUI(FrameInfo& info)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui::NewFrame();

        // user will put UI code elsewhere; simple example:
        ImGui::Begin("ImGui");
        ImGui::Text("Frame time: %.3f ms", info.FrameTime);
        ImGui::End();

        ImGui::Render();
        
        VkClearValue clearValue{};
        clearValue.color = { 0.0f, 0.0f, 0.0f, 0.0f };

        VkRenderPassBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        beginInfo.renderPass = m_Renderpass->GetHandle();
        beginInfo.framebuffer = m_FrameBuffers[info.FrameIndex]->GetHandle();
        beginInfo.renderArea.extent.width = m_Width;
        beginInfo.renderArea.extent.height = m_Height;
        beginInfo.clearValueCount = 1;
        beginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(info.CommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), info.CommandBuffer);
        vkCmdEndRenderPass(info.CommandBuffer);
    }

    void ImGuiRenderer::CreateFramebuffers()
    {
        const Vulkan::Swapchain& swapchain = *Vulkan::Context::Get().m_Swapchain;
        
        m_FrameBuffers.reserve(swapchain.GetImageCount());
        for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
        {
            const Vulkan::FrameBuffer& fb = swapchain.GetFramebuffer(i);
            m_FrameBuffers[i] = CreateScope<Vulkan::FrameBuffer>(VK_NULL_HANDLE, fb.GetImage(), swapchain.GetExtent(), fb.GetFormat(), m_Renderpass->GetHandle());
        }
    }
}
