#include "mxpch.hpp"
#include "Platform/Vulkan/Buffer/Frame.hpp"

#include "Platform/Vulkan/Context.hpp"
#include "Platform/Vulkan/Image.hpp"

namespace Mixture::Vulkan
{
	FrameBuffer::FrameBuffer(const VkImageView depthAttachment, const VkImage swapchainImage, const VkExtent2D extent, const VkFormat format, const VkRenderPass renderPass)
		: m_Format(format), m_Image(swapchainImage)
	{
		m_ImageView = CreateScope<ImageView>(m_Image, m_Format, VK_IMAGE_ASPECT_COLOR_BIT);

		std::vector attachments = { m_ImageView->GetHandle() };
		if (depthAttachment) attachments.push_back(depthAttachment);

		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = renderPass ? renderPass : Context::Get().Swapchain().GetRenderpass().GetHandle();
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.width = extent.width;
		createInfo.height = extent.height;
		createInfo.layers = 1;

		VK_ASSERT(vkCreateFramebuffer(Context::Get().Device().GetHandle(), &createInfo, nullptr, &m_FrameBuffer), "Failed to create VkFramebuffer")
	}

	FrameBuffer::~FrameBuffer()
	{
		if (m_FrameBuffer)
		{
			vkDestroyFramebuffer(Context::Get().Device().GetHandle(), m_FrameBuffer, nullptr);
			m_FrameBuffer = nullptr;
		}

		m_ImageView = nullptr;
	}
}
