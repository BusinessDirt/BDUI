#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class DescriptorPoolSizes
    {
    public:
        DescriptorPoolSizes() = default;
        
        static DescriptorPoolSizes CreateDefault(uint32_t count);
        
        void Add(VkDescriptorType type, uint32_t count) { m_Sizes[type] += count; }
        void Add(const std::vector<VkDescriptorPoolSize>& sizes);
        
        void Clear() { m_Sizes.clear(); }
        
        std::vector<VkDescriptorPoolSize> GetPoolSizes() const;
        uint32_t MaxSets() const;
        
    private:
        std::unordered_map<VkDescriptorType, uint32_t> m_Sizes;
    };
}
