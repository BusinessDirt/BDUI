#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Descriptor/Set.hpp"

namespace Mixture::Vulkan
{
    class DescriptorPool
    {
    public:
        OPAL_NON_COPIABLE(DescriptorPool);
        
        DescriptorPool();
        ~DescriptorPool();
        
        OPAL_NODISCARD DescriptorSet AllocateGlobalSet(const Ref<DescriptorSetLayout>& layout) const;
        OPAL_NODISCARD DescriptorSet AllocateFrameSet(const Ref<DescriptorSetLayout>& layout, uint32_t frameIndex) const;
        
        void ResetFramePool(uint32_t frameIndex) const;
        void FreeGlobalSet(const DescriptorSet& set) const;
        
        OPAL_NODISCARD VkDescriptorPool GetGlobalHandle() const { return m_GlobalPool; }
        OPAL_NODISCARD VkDescriptorPool GetFrameHandle(const uint32_t frameIndex) const { return m_FramePools[frameIndex]; }
        
    private:
        OPAL_NODISCARD VkDescriptorPool CreatePool(uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes,
                                    VkDescriptorPoolCreateFlags flags) const;
        
    private:
        VkDevice m_Device;
        uint32_t m_FramesInFlight;
        
        VkDescriptorPool m_GlobalPool;
        std::vector<VkDescriptorPool> m_FramePools;
    };
}
