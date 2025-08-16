#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Assets/SPVShader.hpp"

namespace Mixture
{
	namespace ShaderCompiler
	{
		enum PipelineType
		{
			GRAPHICS_PIPELINE
		};

		struct Flags
		{
			PipelineType PipelineType = GRAPHICS_PIPELINE;
			bool Debug = true;
		};

		SPVShader Compile(const std::string& shaderName, const Flags& compileFlags);

		static void Reflect(SPVShader& shader);
		static void DebugPrint(const std::string& shaderName, const Flags& compileFlags, SPVShader& shader);
		static std::vector<uint32_t> CompileStage(const std::filesystem::path& path, const Flags& compileFlags, ShaderStage stage);
	}
}
