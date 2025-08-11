#include "Platform/Vulkan/DescriptorPool.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan
{
    DescriptorPool::DescriptorPool()
    {
        std::vector<VkDescriptorPoolSize> poolSizes = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        
        uint32_t poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * poolSizeCount;
        poolInfo.poolSizeCount = poolSizeCount;
        poolInfo.pPoolSizes = poolSizes.data();
        VK_ASSERT(vkCreateDescriptorPool(Context::Get().Device().GetHandle(), &poolInfo, nullptr, &m_Pool), "Failed to create Descriptor Pool");
    }

    // Allocates a descriptor set from this pool for the given layout
    VkDescriptorSet DescriptorPool::AllocateDescriptorSet(VkDescriptorSetLayout layout)
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_Pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;

        VkDescriptorSet descriptorSet;
        VK_ASSERT(vkAllocateDescriptorSets(Context::Get().Device().GetHandle(), &allocInfo, &descriptorSet),
                  "Failed to allocate descriptor set");

        return descriptorSet;
    }

    void DescriptorPool::FreeDescriptorSets(const std::vector<VkDescriptorSet>& descriptorSets)
    {
        if (descriptorSets.empty()) return;

        VK_ASSERT(vkFreeDescriptorSets(Context::Get().Device().GetHandle(), m_Pool,
                                       static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data()),
                  "Failed to free descriptor sets");
    }

    void DescriptorPool::FreeDescriptorSet(VkDescriptorSet descriptorSet)
    {
        FreeDescriptorSets({ descriptorSet });
    }

    // Reset the pool (frees all allocated descriptor sets)
    void DescriptorPool::Reset()
    {
        VK_ASSERT(vkResetDescriptorPool(Context::Get().Device().GetHandle(), m_Pool, 0),
                  "Failed to reset descriptor pool");
    }

    void DescriptorPool::Cleanup()
    {
        if (m_Pool != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(Vulkan::Context::Get().Device().GetHandle(), m_Pool, nullptr);
            m_Pool = VK_NULL_HANDLE;
        }
    }
}
