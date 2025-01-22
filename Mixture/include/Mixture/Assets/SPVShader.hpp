#pragma once

#include "Mixture/Core/Base.hpp"

#include <vulkan/vulkan.h>

namespace Mixture
{
	enum ShaderStage
	{
		SHADER_STAGE_VERTEX = 0x00000001,
		SHADER_STAGE_FRAGMENT = 0x00000010
	};

	struct SPVShader
	{
		VkPushConstantRange PushConstant{};
		std::vector<VkVertexInputBindingDescription> VertexInputBindings;
		std::vector<VkVertexInputAttributeDescription> VertexInputAttributes;
        std::vector<std::string> VertexAttributeNames;
		std::unordered_map<ShaderStage, std::vector<uint32_t>> Data;
	};

	namespace Util
	{
		ShaderStage FilePathToShaderStage(const std::filesystem::path& filePath);
		std::string ShaderStageToString(ShaderStage stage);
	}
}
