#include "mxpch.hpp"
#include "Platform/Vulkan/Descriptor/Set.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan
{
    DescriptorSet::DescriptorSet(VkDescriptorPool pool, const std::vector<DescriptorBinding>& bindings)
    {
        m_Device = Context::Get().Device().GetHandle();
        m_Layout = CreateScope<DescriptorSetLayout>(bindings);
        VkDescriptorSetLayout layoutHandle = m_Layout->GetHandle();
        
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layoutHandle;

        VK_ASSERT(vkAllocateDescriptorSets(m_Device, &allocInfo, &m_Set), "Failed to allocate DescriptorSet");
    }

    void DescriptorSet::Free(VkDescriptorPool pool)
    {
        vkFreeDescriptorSets(m_Device, pool, 1, &m_Set);
    }

    void DescriptorSet::UpdateBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_Set;
        write.dstBinding = binding;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // inferred here, but could be stored in layout
        write.pBufferInfo = bufferInfo;

        vkUpdateDescriptorSets(m_Device, 1, &write, 0, nullptr);
    }

    void DescriptorSet::UpdateImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_Set;
        write.dstBinding = binding;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // inferred here, but could be stored in layout
        write.pImageInfo = imageInfo;

        vkUpdateDescriptorSets(m_Device, 1, &write, 0, nullptr);
    }
}
