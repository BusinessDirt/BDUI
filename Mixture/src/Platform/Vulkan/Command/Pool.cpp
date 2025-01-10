#include "mxpch.hpp"
#include "Platform/Vulkan/Command/Pool.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Vulkan
{
    CommandPool::CommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = Context::Get().m_PhysicalDevice->GetQueueFamilyIndices();

        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = queueFamilyIndices.Graphics.value();

        VK_ASSERT(vkCreateCommandPool(Context::Get().m_Device->GetHandle(), &createInfo, nullptr, &m_CommandPool), "Failed to create VkCommandPool");
    }

    CommandPool::~CommandPool()
    {
        if (m_CommandPool)
        {
            vkDestroyCommandPool(Context::Get().m_Device->GetHandle(), m_CommandPool, nullptr);
            m_CommandPool = nullptr;
        }
    }
}