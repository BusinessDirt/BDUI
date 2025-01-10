#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Mixture/Assets/SPVShader.hpp"

namespace Vulkan
{
	class ShaderModule
	{
	public:
		ShaderModule(const Mixture::SPVShader& shader, Mixture::ShaderStage stage);
		~ShaderModule();

		VkPipelineShaderStageCreateInfo CreateInfo();

	private:
		VULKAN_HANDLE(VkShaderModule, m_ShaderModule);
		Mixture::ShaderStage m_Stage;
	};
}