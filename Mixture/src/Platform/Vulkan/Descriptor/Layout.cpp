#include "mxpch.hpp"
#include "Platform/Vulkan/Descriptor/Layout.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture::Vulkan
{
    DescriptorSetLayout::DescriptorSetLayout(const std::vector<DescriptorBinding>& bindings)
        : m_Bindings(bindings)
    {
        m_Device = Context::Get().Device().GetHandle();
        std::vector<VkDescriptorSetLayoutBinding> vkBindings;
        vkBindings.reserve(bindings.size());

        for (const auto& b : bindings) {
            VkDescriptorSetLayoutBinding vkBinding{};
            vkBinding.binding = b.Binding;
            vkBinding.descriptorType = b.Type;
            vkBinding.descriptorCount = b.Count;
            vkBinding.stageFlags = b.StageFlags;
            vkBindings.push_back(vkBinding);
        }

        VkDescriptorSetLayoutCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.bindingCount = static_cast<uint32_t>(vkBindings.size());
        info.pBindings = vkBindings.data();

        VK_ASSERT(vkCreateDescriptorSetLayout(m_Device, &info, nullptr, &m_Layout), "Failed to create DescriptorSetLayout");
    }

    DescriptorSetLayout::~DescriptorSetLayout() {
        if (m_Layout) vkDestroyDescriptorSetLayout(m_Device, m_Layout, nullptr);
    }
}
