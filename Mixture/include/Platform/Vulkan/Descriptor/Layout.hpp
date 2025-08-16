#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    struct DescriptorBinding
    {
        uint32_t Binding;
        VkDescriptorType Type;
        VkShaderStageFlags StageFlags;
        uint32_t Count = 1;
    };

    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(const std::vector<DescriptorBinding>& bindings);
        ~DescriptorSetLayout();

        const std::vector<DescriptorBinding>& GetBindings() const { return m_Bindings; }

    private:
        VULKAN_HANDLE(VkDescriptorSetLayout, m_Layout);
        VkDevice m_Device;
        std::vector<DescriptorBinding> m_Bindings;
    };
}
