#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Descriptor/Layout.hpp"

namespace Mixture::Vulkan
{
    class DescriptorSet
    {
    public:
        DescriptorSet(VkDescriptorPool pool, const Ref<DescriptorSetLayout>& layout);
        void Free(VkDescriptorPool pool) const;
        
        void UpdateBuffer(uint32_t binding, const VkDescriptorBufferInfo* bufferInfo) const;
        void UpdateImage(uint32_t binding, const VkDescriptorImageInfo* imageInfo) const;

        const DescriptorSetLayout& GetLayout() const { return *m_Layout; }

    private:
        VULKAN_HANDLE(VkDescriptorSet, m_Set);
        Ref<DescriptorSetLayout> m_Layout;
        
        VkDevice m_Device;
    };
}
