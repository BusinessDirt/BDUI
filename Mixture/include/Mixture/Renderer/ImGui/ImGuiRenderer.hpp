#pragma once
#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/RendererInfo.hpp"

#include "Platform/Vulkan/Renderpass.hpp"
#include "Platform/Vulkan/Swapchain.hpp"
#include "Platform/Vulkan/Buffer/Frame.hpp"

#include <vulkan/vulkan.h>
#include <imgui.h>

namespace Mixture
{
    class ImGuiRenderer
    {
    public:
        ImGuiRenderer() = default;
        ~ImGuiRenderer();
        
        void Initialize();
        void Shutdown();

        void OnFramebufferResize(uint32_t width, uint32_t height);

        void BeginFrame();
        void EndFrame();
        
        void BeginRenderpass(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);
        void EndRenderpass(VkCommandBuffer commandBuffer);
        
    private:
        void CreateFramebuffers();

    private:
        Scope<Vulkan::Renderpass> m_Renderpass;
        std::vector<Scope<Vulkan::FrameBuffer>> m_FrameBuffers;
        
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
}
