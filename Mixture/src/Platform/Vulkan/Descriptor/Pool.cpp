#include "mxpch.hpp"
#include "Platform/Vulkan/Descriptor/Pool.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan
{
    DescriptorPool::DescriptorPool()
    {
        m_Device = Context::Get().Device().GetHandle();
        m_FramesInFlight = Swapchain::MAX_FRAMES_IN_FLIGHT;
        
        // ---- Create Global Pool ----
        std::vector<VkDescriptorPoolSize> globalPoolSizes = {
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 500 }
        };
        
        m_GlobalPool = CreatePool(600, globalPoolSizes, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        
        // --- Create Per-Frame Pools ---
        std::vector<VkDescriptorPoolSize> framePoolSizes = {
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 200 }
        };

        m_FramePools.resize(m_FramesInFlight);
        for (uint32_t i = 0; i < m_FramesInFlight; i++)
        {
            m_FramePools[i] = CreatePool(200, framePoolSizes, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        }
    }

    DescriptorPool::~DescriptorPool()
    {
        for (auto pool : m_FramePools) vkDestroyDescriptorPool(m_Device, pool, nullptr);
        vkDestroyDescriptorPool(m_Device, m_GlobalPool, nullptr);
    }

    VkDescriptorPool DescriptorPool::CreatePool(uint32_t maxSets, const std::vector<VkDescriptorPoolSize> &poolSizes, VkDescriptorPoolCreateFlags flags)
    {
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.maxSets = maxSets;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.flags = flags;

        VkDescriptorPool pool;
        VK_ASSERT(vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &pool), "Failed to create Descriptor Pool");
        return pool;
    }

    DescriptorSet DescriptorPool::AllocateGlobalSet(Ref<DescriptorSetLayout> layout) const
    {
        return DescriptorSet(m_GlobalPool, layout);
    }

    DescriptorSet DescriptorPool::AllocateFrameSet(Ref<DescriptorSetLayout> layout, uint32_t frameIndex) const
    {
        return DescriptorSet(m_FramePools[frameIndex], layout);
    }

    void DescriptorPool::ResetFramePool(uint32_t frameIndex) const
    {
        vkResetDescriptorPool(m_Device, m_FramePools[frameIndex], 0);
    }
}
