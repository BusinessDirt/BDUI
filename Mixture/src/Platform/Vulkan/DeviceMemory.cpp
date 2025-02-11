#include "mxpch.hpp"
#include "Platform/Vulkan/DeviceMemory.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan 
{

    DeviceMemory::DeviceMemory(const size_t size, const uint32_t memoryTypeBits, const VkMemoryAllocateFlags allocateFLags, const VkMemoryPropertyFlags propertyFlags)
    {
        VkMemoryAllocateFlagsInfo flagsInfo = {};
        flagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
        flagsInfo.pNext = nullptr;
        flagsInfo.flags = allocateFLags;

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = &flagsInfo;
        allocInfo.allocationSize = size;
        allocInfo.memoryTypeIndex = FindMemoryType(memoryTypeBits, propertyFlags);

        VK_ASSERT(vkAllocateMemory(Context::Get().m_Device->GetHandle(), &allocInfo, nullptr, &m_Memory), "Failed to allocate VkDeviceMemory!");
    }

    DeviceMemory::DeviceMemory(DeviceMemory&& other) noexcept
        : m_Memory(other.m_Memory)
    {
        other.m_Memory = nullptr;
    }

    DeviceMemory::~DeviceMemory()
    {
        if (m_Memory)
        {
            vkFreeMemory(Context::Get().m_Device->GetHandle(), m_Memory, nullptr);
            m_Memory = nullptr;
        }
    }

    void* DeviceMemory::Map(const size_t offset, const size_t size)
    {
        void* data;
        VK_ASSERT(vkMapMemory(Context::Get().m_Device->GetHandle(), m_Memory, offset, size, 0, &data), "Failed to map VkDeviceMemory!");

        return data;
    }

    void DeviceMemory::Unmap()
    {
        vkUnmapMemory(Context::Get().m_Device->GetHandle(), m_Memory);
    }

    uint32_t DeviceMemory::FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags propertyFlags) const
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(Context::Get().m_PhysicalDevice->GetHandle(), &memProperties);

        for (uint32_t i = 0; i != memProperties.memoryTypeCount; ++i)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
            {
                return i;
            }
        }

        OPAL_CORE_ERROR("Failed to find suitable VkDeviceMemory type!");
        return 0;
    }

}