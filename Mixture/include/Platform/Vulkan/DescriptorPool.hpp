#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class DescriptorPool
    {
    public:
        DescriptorPool();
        ~DescriptorPool() { Cleanup(); }
        
        // No copying or assignment
        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;

        // Allow move semantics
        DescriptorPool(DescriptorPool&& other) noexcept : m_Pool(other.m_Pool) { other.m_Pool = VK_NULL_HANDLE; }
        DescriptorPool& operator=(DescriptorPool&& other) noexcept
        {
            if (this != &other)
            {
                Cleanup();
                m_Pool = other.m_Pool;
                other.m_Pool = VK_NULL_HANDLE;
            }
            return *this;
        }
        
        VkDescriptorSet AllocateDescriptorSet(VkDescriptorSetLayout layout);
        void FreeDescriptorSets(const std::vector<VkDescriptorSet>& descriptorSets);
        void FreeDescriptorSet(VkDescriptorSet descriptorSet);
        void Reset();
        
    private:
        void Cleanup();
        
    private:
        VULKAN_HANDLE(VkDescriptorPool, m_Pool);
    };
}
