#include "mxpch.hpp"
#include "Mixture/Assets/SPVShader.hpp"

namespace Mixture::Util
{
	ShaderStage FilePathToShaderStage(const std::filesystem::path& filePath)
	{
		const std::string extension = filePath.extension().string();
		if (extension == ".vert") return ShaderStageVertex;
		if (extension == ".frag") return ShaderStageFragment;

		OPAL_CORE_ERROR("Unknown shader file extension '{0}'", filePath.string().c_str());
		return ShaderStageVertex;
	}

	std::string ShaderStageToString(const ShaderStage stage)
	{
		if (stage == ShaderStageFragment) return "Fragment Shader Stage";
		if (stage == ShaderStageVertex) return "Vertex Shader Stage";
		return "Unknown Shader Stage";
	}
}

