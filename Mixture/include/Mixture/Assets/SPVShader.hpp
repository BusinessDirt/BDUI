#pragma once

#include "Mixture/Core/Base.hpp"

#include <vulkan/vulkan.h>

namespace Mixture
{
	enum ShaderStage : uint8_t
	{
		ShaderStageVertex = 0x00000001,
		ShaderStageFragment = 0x00000010
	};

	struct SpvShader
	{
		VkPushConstantRange PushConstant{};
        std::unordered_map<uint32_t, std::vector<VkDescriptorSetLayoutBinding>> DescriptorSetLayoutBindings;
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
