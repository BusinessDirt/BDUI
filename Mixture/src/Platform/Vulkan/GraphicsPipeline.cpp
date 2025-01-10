#include "mxpch.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

#include "Mixture/Assets/ShaderCompiler.hpp"

#include "Platform/Vulkan/ShaderModule.hpp"

namespace Vulkan
{
    GraphicsPipeline::GraphicsPipeline(const VkDevice device)
        : m_Device(device)
    {
        Mixture::ShaderCompiler::Flags flags{};
        flags.PipelineType = Mixture::ShaderCompiler::GRAPHICS_PIPELINE;
        flags.Debug = true;

        Mixture::SPVShader shader = Mixture::ShaderCompiler::Compile("shader", flags);
        ShaderModule vertShader(m_Device, shader, Mixture::SHADER_STAGE_VERTEX);
        ShaderModule fragShader(m_Device, shader, Mixture::SHADER_STAGE_FRAGMENT);
        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShader.CreateInfo(), fragShader.CreateInfo() };
    };

    GraphicsPipeline::~GraphicsPipeline()
    {
        
    };
}
