#include "mxpch.hpp"
#include "Platform/Vulkan/Buffer/Uniform.hpp"

namespace Mixture::Vulkan
{
    UniformBuffer::UniformBuffer(uint32_t instanceSize, uint32_t instanceCount)
    {
        m_Buffer = CreateScope<Buffer>(instanceSize, instanceCount, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        m_DescriptorInfo = {};
    }

    UniformBuffer::~UniformBuffer()
    {
        m_Buffer.reset();
    }

    void UniformBuffer::SetData(void* data, uint32_t index) const
    {
        uint32_t size = m_Buffer->GetInstanceSize();
        m_Buffer->Map();
        m_Buffer->WriteToBuffer(data, size, index * size);
        m_Buffer->Unmap();
    }

    const VkDescriptorBufferInfo* UniformBuffer::GetDescriptorInfo(uint32_t index)
    {
        uint32_t size = m_Buffer->GetInstanceSize();
        m_DescriptorInfo.buffer = m_Buffer->GetHandle();
        m_DescriptorInfo.offset = size * index;
        m_DescriptorInfo.range  = size;
        return &m_DescriptorInfo;
    }
}


