#include "Platform/Vulkan/Descriptor/PoolSizes.hpp"

namespace Mixture::Vulkan
{
    DescriptorPoolSizes DescriptorPoolSizes::CreateDefault(uint32_t count)
    {
        DescriptorPoolSizes poolSizes = DescriptorPoolSizes();
        poolSizes.Add(VK_DESCRIPTOR_TYPE_SAMPLER, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count);
        poolSizes.Add(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count);
        return poolSizes;
    }

    void DescriptorPoolSizes::Add(const std::vector<VkDescriptorPoolSize>& sizes)
    {
        for (const auto& size : sizes)
        {
            Add(size.type, size.descriptorCount);
        }
    }

    std::vector<VkDescriptorPoolSize> DescriptorPoolSizes::GetPoolSizes() const
    {
        std::vector<VkDescriptorPoolSize> poolSizes;
        poolSizes.reserve(m_Sizes.size());
        for (const auto& [type, count] : m_Sizes) poolSizes.push_back({ type, count });
        return poolSizes;
    }

    uint32_t DescriptorPoolSizes::MaxSets() const
    {
        uint32_t total = 0;
        for (const auto& [_, count] : m_Sizes) total += count;
        return total;
    }
}
