#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Image.hpp"

namespace Vulkan
{
	class ImageView;

	class FrameBuffer
	{
	public:
		OPAL_NON_COPIABLE(FrameBuffer);

		FrameBuffer(VkImageView depthAttachment, VkImage swapchainImage, VkExtent2D extent, VkFormat format, VkRenderPass renderPass = nullptr);
		~FrameBuffer();

		VkFormat GetFormat() const { return m_Format; }
		const VkImage GetImage() const { return m_Image; }
		const ImageView& GetImageView() const { return *m_ImageView; }

	private:
		VULKAN_HANDLE(VkFramebuffer, m_FrameBuffer);

		const VkFormat m_Format;
		const VkImage m_Image;
		std::unique_ptr<ImageView> m_ImageView; 
	};
}