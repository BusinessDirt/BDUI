#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Descriptor/Set.hpp"

namespace Mixture::Vulkan
{
    class DescriptorPool
    {
    public:
        DescriptorPool();
        ~DescriptorPool();
        
        DescriptorSet AllocateGlobalSet(Ref<DescriptorSetLayout> layout) const;
        DescriptorSet AllocateFrameSet(Ref<DescriptorSetLayout> layout, uint32_t frameIndex) const;
        
        void ResetFramePool(uint32_t frameIndex) const;
        void FreeGlobalSet(DescriptorSet& set) const;
        
        VkDescriptorPool GetGlobalHandle() const { return m_GlobalPool; }
        VkDescriptorPool GetFrameHandle(uint32_t frameIndex) const { return m_FramePools[frameIndex]; }
        
    private:
        VkDescriptorPool CreatePool(uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes,
                                    VkDescriptorPoolCreateFlags flags);
        
    private:
        VkDevice m_Device;
        uint32_t m_FramesInFlight;
        
        VkDescriptorPool m_GlobalPool;
        std::vector<VkDescriptorPool> m_FramePools;
    };
}
