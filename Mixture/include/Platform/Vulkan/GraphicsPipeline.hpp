#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Renderpass.hpp"
#include "Platform/Vulkan/Swapchain.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"

namespace Mixture::Vulkan
{
    class GraphicsPipeline
    {
    public:
        OPAL_NON_COPIABLE(GraphicsPipeline);

        GraphicsPipeline(const std::string& shaderName);
        ~GraphicsPipeline();

        void Bind(VkCommandBuffer commandBuffer);
        void PushConstants(VkCommandBuffer commandBuffer, const void* pValues);

    private:
        VULKAN_HANDLE(VkPipeline, m_GraphicsPipeline);
        VkPushConstantRange m_PushConstant;
        VkPipelineLayout m_PipelineLayout;
    };
}
