#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Descriptor/Layout.hpp"

namespace Mixture::Vulkan
{
    class DescriptorSet
    {
    public:
        DescriptorSet(VkDescriptorPool pool, const std::vector<DescriptorBinding>& bindings);
        void Free(VkDescriptorPool pool);
        
        void UpdateBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        void UpdateImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        const DescriptorSetLayout& GetLayout() const { return *m_Layout; }

    private:
        VULKAN_HANDLE(VkDescriptorSet, m_Set);
        Scope<DescriptorSetLayout> m_Layout;
        
        VkDevice m_Device;
    };
}
