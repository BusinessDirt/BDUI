#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Renderpass.hpp"
#include "Platform/Vulkan/Swapchain.hpp"
#include "Platform/Vulkan/Descriptor/Layout.hpp"
#include "Platform/Vulkan/Descriptor/Set.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"

namespace Mixture::Vulkan
{
    class GraphicsPipeline
    {
    public:
        OPAL_NON_COPIABLE(GraphicsPipeline);

        GraphicsPipeline(const std::string& shaderName);
        ~GraphicsPipeline();

        void Bind(VkCommandBuffer commandBuffer) const;
        void PushConstants(VkCommandBuffer commandBuffer, const void* pValues) const;
        void UpdateGlobalUniformBuffer(const VkDescriptorBufferInfo* bufferInfo) const;

    private:
        VULKAN_HANDLE(VkPipeline, m_GraphicsPipeline);
        VkPipelineLayout m_PipelineLayout;
        
        VkPushConstantRange m_PushConstant;
        std::vector<Ref<DescriptorSetLayout>> m_SetLayouts;
        
        Scope<DescriptorSet> m_GlobalSet;
    };
}
