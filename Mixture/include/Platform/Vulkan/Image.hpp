#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/DeviceMemory.hpp"

namespace Mixture::Vulkan
{
    class Buffer;

    class Image final
    {
    public:
        Image(const Image&) = delete;
        Image& operator = (const Image&) = delete;
        Image& operator = (Image&&) = delete;

        Image(VkExtent2D extent, VkFormat format, uint32_t mipLevels = 1);
        Image(VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, uint32_t mipLevels = 1);
        Image(Image&& other) noexcept;
        ~Image();

        VkExtent2D GetExtent() const { return m_Extent; }
        VkFormat GetFormat() const { return m_Format; }
        uint32_t GetMipLevels() const { return m_MipLevels; }

        DeviceMemory AllocateMemory(VkMemoryPropertyFlags properties) const;
        VkMemoryRequirements GetMemoryRequirements() const;

        void TransitionImageLayout(VkImageLayout newLayout);
        void GenerateMipMaps(VkFormat imageFormat);
        void CopyFrom(const Buffer& buffer);

    private:
        uint32_t m_MipLevels;
        const VkExtent2D m_Extent;
        const VkFormat m_Format;
        VkImageLayout m_ImageLayout;

        VULKAN_HANDLE(VkImage, m_Image);
    };

    class ImageView final
    {
    public:
        OPAL_NON_COPIABLE(ImageView);

        explicit ImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels = 1);
        ~ImageView();

    private:
        VULKAN_HANDLE(VkImageView, m_ImageView);

        const VkImage m_Image;
        const VkFormat m_Format;
    };
}
