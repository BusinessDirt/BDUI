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
        
        DescriptorSet AllocateGlobalSet(const std::vector<DescriptorBinding>& bindings);
        DescriptorSet AllocateFrameSet(const std::vector<DescriptorBinding>& bindings, uint32_t frameIndex);
        
        void ResetFramePool(uint32_t frameIndex);
        void FreeGlobalSet(DescriptorSet& set);
        
        void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t firstSet, const std::vector<VkDescriptorSet>& sets, const std::vector<uint32_t>& dynamicOffsets = {});
        
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
