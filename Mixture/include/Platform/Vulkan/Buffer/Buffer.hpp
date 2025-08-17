#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class Buffer
    {
    public:
        OPAL_NON_COPIABLE(Buffer);
        
        Buffer(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
        ~Buffer();

        VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void Unmap();

        void WriteToBuffer(const void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        OPAL_NODISCARD VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        OPAL_NODISCARD VkDescriptorBufferInfo GetDescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
        OPAL_NODISCARD VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

        void WriteToIndex(const void* data, int index) const;
        OPAL_NODISCARD VkResult FlushIndex(int index) const;
        OPAL_NODISCARD VkDescriptorBufferInfo GetDescriptorInfoForIndex(int index) const;
        OPAL_NODISCARD VkResult InvalidateIndex(int index) const;

        OPAL_NODISCARD void* GetMappedMemory() const { return m_Mapped; }
        OPAL_NODISCARD uint32_t GetInstanceCount() const { return m_InstanceCount; }
        OPAL_NODISCARD VkDeviceSize GetInstanceSize() const { return m_InstanceSize; }
        OPAL_NODISCARD VkDeviceSize GetAlignmentSize() const { return m_InstanceSize; }
        OPAL_NODISCARD VkBufferUsageFlags GetUsageFlags() const { return m_UsageFlags; }
        OPAL_NODISCARD VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return m_MemoryPropertyFlags; }
        OPAL_NODISCARD VkDeviceSize GetBufferSize() const { return m_BufferSize; }

        static void Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandBuffer commandBuffer = VK_NULL_HANDLE);

    private:
        static VkDeviceSize GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);
        static void Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

        void* m_Mapped = nullptr;
        VULKAN_HANDLE(VkBuffer, m_Buffer);
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;

        VkDeviceSize m_BufferSize;
        uint32_t m_InstanceCount;
        VkDeviceSize m_InstanceSize;
        VkDeviceSize m_AlignmentSize;
        VkBufferUsageFlags m_UsageFlags;
        VkMemoryPropertyFlags m_MemoryPropertyFlags;
    };
}
