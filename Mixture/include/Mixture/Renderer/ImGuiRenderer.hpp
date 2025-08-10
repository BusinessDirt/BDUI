#pragma once
#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/RendererInfo.hpp"

#include "Platform/Vulkan/Renderpass.hpp"
#include "Platform/Vulkan/Buffer/Frame.hpp"

#include <vulkan/vulkan.h>


namespace Mixture
{
    class ImGuiRenderer
    {
    public:
        ImGuiRenderer() = default;
        ~ImGuiRenderer();
        
        void Initialize();
        void Shutdown();

        void OnWindowResize(uint32_t width, uint32_t height);

        void RenderUI(FrameInfo& info);
        
    private:
        void CreateFramebuffers();

    private:
        Scope<Vulkan::Renderpass> m_Renderpass;
        std::vector<Scope<Vulkan::FrameBuffer>> m_FrameBuffers;
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
        
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
}
