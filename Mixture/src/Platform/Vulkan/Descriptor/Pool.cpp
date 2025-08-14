#include "mxpch.hpp"
#include "Platform/Vulkan/Descriptor/Pool.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan
{
    DescriptorPool::DescriptorPool(DescriptorPoolSizes poolSizes)
    {
        std::vector<VkDescriptorPoolSize> sizes = poolSizes.GetPoolSizes();
        
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = poolSizes.MaxSets();
        poolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
        poolInfo.pPoolSizes = sizes.data();
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
