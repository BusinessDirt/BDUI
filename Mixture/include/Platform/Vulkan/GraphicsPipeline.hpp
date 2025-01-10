#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Renderpass.hpp"
#include "Platform/Vulkan/Swapchain.hpp"

#include "Mixture/Core/FrameInfo.hpp"

namespace Vulkan
{
    class GraphicsPipeline
    {
    public:
        OPAL_NON_COPIABLE(GraphicsPipeline);

        GraphicsPipeline();
        ~GraphicsPipeline();

        void Bind(const FrameInfo& frameInfo);
        void PushConstants(const FrameInfo& info, const void* pValues);

    private:
        VULKAN_HANDLE(VkPipeline, m_GraphicsPipeline);
        VkPushConstantRange m_PushConstant;
        VkPipelineLayout m_PipelineLayout;
    };
}
