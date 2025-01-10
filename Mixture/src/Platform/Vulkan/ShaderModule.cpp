#include "mxpch.hpp"
#include "Platform/Vulkan/ShaderModule.hpp"

namespace Vulkan
{
	ShaderModule::ShaderModule(const VkDevice device, const Mixture::SPVShader& shader, Mixture::ShaderStage stage)
		: m_Device(device), m_Stage(stage)
	{
		if (shader.Data.contains(stage))
		{
			OPAL_CORE_ERROR("SPVShader doesn't contain code for stage '{}'", Mixture::Util::ShaderStageToString(stage));
			OPAL_CORE_ASSERT(false);
		}

		const std::vector<uint32_t>& code = shader.Data.at(stage);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VK_ASSERT(vkCreateShaderModule(device, &createInfo, nullptr, &m_ShaderModule), "Failed to create VkShaderModule!");
	}

	ShaderModule::~ShaderModule()
	{
		if (m_ShaderModule)
		{
			vkDestroyShaderModule(m_Device, m_ShaderModule, nullptr);
			m_ShaderModule = nullptr;
		}
	}

	VkPipelineShaderStageCreateInfo ShaderModule::CreateInfo()
	{
		VkPipelineShaderStageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfo.stage = (VkShaderStageFlagBits)m_Stage;
		createInfo.module = m_ShaderModule;
		createInfo.pName = "main";
		return createInfo;
	}
}
