#include "mxpch.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

#include "Mixture/Assets/ShaderCompiler.hpp"

#include "Platform/Vulkan/ShaderModule.hpp"
#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan
{
    GraphicsPipeline::GraphicsPipeline(const std::string& shaderName)
    {
        const Swapchain& swapchain = Context::Get().Swapchain();

        Mixture::ShaderCompiler::Flags flags{};
        flags.PipelineType = Mixture::ShaderCompiler::GRAPHICS_PIPELINE;
        flags.Debug = true;

        Mixture::SPVShader shader = Mixture::ShaderCompiler::Compile(shaderName, flags);
        ShaderModule vertShader(shader, Mixture::SHADER_STAGE_VERTEX);
        ShaderModule fragShader(shader, Mixture::SHADER_STAGE_FRAGMENT);
        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShader.CreateInfo(), fragShader.CreateInfo() };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(shader.VertexInputBindings.size());
        vertexInputInfo.pVertexBindingDescriptions = shader.VertexInputBindings.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(shader.VertexInputAttributes.size());
        vertexInputInfo.pVertexAttributeDescriptions = shader.VertexInputAttributes.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapchain.GetExtent().width;
        viewport.height = (float)swapchain.GetExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapchain.GetExtent();

        std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;

        if (shader.PushConstant.size > 0)
        {
            m_PushConstant = shader.PushConstant;
            pipelineLayoutInfo.pushConstantRangeCount = 1;
            pipelineLayoutInfo.pPushConstantRanges = &shader.PushConstant;
        }

        VK_ASSERT(vkCreatePipelineLayout(Context::Get().Device().GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout), "Failed to create Graphics Pipeline Layout!");

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_PipelineLayout;
        pipelineInfo.renderPass = swapchain.GetRenderpass().GetHandle();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        VK_ASSERT(vkCreateGraphicsPipelines(Context::Get().Device().GetHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline), "Failed to create Graphics Pipeline");
    };

    GraphicsPipeline::~GraphicsPipeline()
    {
        if (m_GraphicsPipeline)
        {
            vkDestroyPipeline(Context::Get().Device().GetHandle(), m_GraphicsPipeline, nullptr);
            m_GraphicsPipeline = nullptr;
        }

        if (m_PipelineLayout)
        {
            vkDestroyPipelineLayout(Context::Get().Device().GetHandle(), m_PipelineLayout, nullptr);
            m_PipelineLayout = nullptr;
        }
    }

    void GraphicsPipeline::Bind(const FrameInfo& frameInfo)
    {
        vkCmdBindPipeline(frameInfo.CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
        //std::array<VkDescriptorSet, 2> sets = { frameInfo.GlobalSet, frameInfo.InstanceSet };
        //vkCmdBindDescriptorSets(frameInfo.CommandBuffer.GetAsVulkanHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout->GetHandle(), 0,
        //    static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
    }

    void GraphicsPipeline::PushConstants(const FrameInfo& frameInfo, const void* pValues)
    {
        if (m_PushConstant.size > 0)
        {
            vkCmdPushConstants(frameInfo.CommandBuffer, m_PipelineLayout, m_PushConstant.stageFlags,
                m_PushConstant.offset, m_PushConstant.size, pValues);
        }
        else
        {
            OPAL_CORE_ERROR("Shader doesn't use push constant!");
        } 
    }
}
