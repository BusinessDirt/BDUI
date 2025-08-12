#pragma once

#include "Mixture/Core/Base.hpp"

#include "Platform/Vulkan/Image.hpp"
#include "Platform/Vulkan/DeviceMemory.hpp"

namespace Mixture
{
    class ImGuiViewport
    {
    public:
        ImGuiViewport() = default;
        
        void Initialize() { CreateResources(); };
        void Shutdown();
        
        void BlitFromScene(VkCommandBuffer cmd, VkImage sceneImage, uint32_t sceneWidth, uint32_t sceneHeight);
        
        void OnFramebufferResize() { m_Resize = true; }
        void Resize();
        void DrawWindow();
        
    private:
        void CreateResources();
        void Cleanup();
        
        void TransitionImageLayout(VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,
                VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage);
        
    private:
        uint32_t m_Width = 1, m_Height = 1;
        uint32_t m_ImageWidth = 1, m_ImageHeight = 1;
        bool m_Resize = true;
        
        Scope<Vulkan::Image> m_Image;
        Scope<Vulkan::ImageView> m_ImageView;
        Scope<Vulkan::DeviceMemory> m_ImageMemory;
        
        VkSampler m_ImageSampler = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
    };
}
