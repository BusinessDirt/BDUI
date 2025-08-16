#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings);
        ~DescriptorSetLayout();

        const std::vector<VkDescriptorSetLayoutBinding>& GetBindings() const { return m_Bindings; }

    private:
        VULKAN_HANDLE(VkDescriptorSetLayout, m_Layout);
        VkDevice m_Device;
        std::vector<VkDescriptorSetLayoutBinding> m_Bindings;
    };
}
