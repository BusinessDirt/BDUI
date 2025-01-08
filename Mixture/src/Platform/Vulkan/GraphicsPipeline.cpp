#include "mxpch.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

#include "Mixture/Assets/ShaderCompiler.hpp"

namespace Vulkan
{
    GraphicsPipeline::GraphicsPipeline()
    {
        Mixture::ShaderCompiler::Flags flags{};
        flags.PipelineType = Mixture::ShaderCompiler::GRAPHICS_PIPELINE;
        flags.Debug = true;

        Mixture::SPVShader shader = Mixture::ShaderCompiler::Compile("shader", flags);
    };

    GraphicsPipeline::~GraphicsPipeline()
    {
        
    };
}
