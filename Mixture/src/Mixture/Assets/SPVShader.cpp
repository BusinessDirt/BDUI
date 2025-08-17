#include "mxpch.hpp"
#include "Mixture/Assets/SPVShader.hpp"

namespace Mixture::Util
{
	ShaderStage FilePathToShaderStage(const std::filesystem::path& filePath)
	{
		const std::string extension = filePath.extension().string();
		if (extension == ".vert") return SHADER_STAGE_VERTEX;
		if (extension == ".frag") return SHADER_STAGE_FRAGMENT;

		OPAL_CORE_ERROR("Unknown shader file extension '{0}'", filePath.string().c_str());
		return SHADER_STAGE_VERTEX;
	}

	std::string ShaderStageToString(const ShaderStage stage)
	{
		if (stage == SHADER_STAGE_FRAGMENT) return "Fragment Shader Stage";
		if (stage == SHADER_STAGE_VERTEX) return "Vertex Shader Stage";
		return "Unknown Shader Stage";
	}
}

