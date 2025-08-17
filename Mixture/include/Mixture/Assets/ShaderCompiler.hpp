#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Assets/SPVShader.hpp"

namespace Mixture::ShaderCompiler
{
	enum PipelineType : uint8_t
	{
		Graphics
	};

	struct Flags
	{
		PipelineType PipelineType = Graphics;
		bool Debug = true;
	};

	SpvShader Compile(const std::string& shaderName, const Flags& compileFlags);

	static void Reflect(SpvShader& shader);
	static void DebugPrint(const std::string& shaderName, const Flags& compileFlags, SpvShader& shader);
	static std::vector<uint32_t> CompileStage(const std::filesystem::path& path, const Flags& compileFlags, ShaderStage stage);
}
