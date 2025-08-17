#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"

namespace Mixture::Vulkan
{
    class UniformBuffer
    {
    public:
        UniformBuffer(uint32_t instanceSize, uint32_t instanceCount = 1);
        ~UniformBuffer();

        void SetData(const void* data, uint32_t index = 0) const;
        const VkDescriptorBufferInfo* GetDescriptorInfo(uint32_t index = 0);

    private:
        Scope<Buffer> m_Buffer;
        VkDescriptorBufferInfo m_DescriptorInfo;
    };
}
