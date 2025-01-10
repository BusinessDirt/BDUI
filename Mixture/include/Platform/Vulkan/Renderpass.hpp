#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Vulkan
{
	class Renderpass
	{
	public:
		Renderpass(VkFormat format, bool useDepth = true, VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		~Renderpass();

	private:
		VULKAN_HANDLE(VkRenderPass, m_Renderpass);
	};
}