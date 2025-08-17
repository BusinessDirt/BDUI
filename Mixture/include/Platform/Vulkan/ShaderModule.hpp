#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Mixture/Assets/SPVShader.hpp"

namespace Mixture::Vulkan
{
	class ShaderModule
	{
	public:
		OPAL_NON_COPIABLE(ShaderModule);
		
		ShaderModule(const SPVShader& shader, ShaderStage stage);
		~ShaderModule();

		VkPipelineShaderStageCreateInfo CreateInfo();

	private:
		VULKAN_HANDLE(VkShaderModule, m_ShaderModule);
		ShaderStage m_Stage;
	};
}