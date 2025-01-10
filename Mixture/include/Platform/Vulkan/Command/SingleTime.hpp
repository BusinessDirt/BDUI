#pragma once

#include "Platform/Vulkan/Base.hpp"

#include "Platform/Vulkan/Device.hpp"
#include "Platform/Vulkan/Context.hpp"
#include "Platform/Vulkan/Command/Pool.hpp"

namespace Vulkan::SingleTimeCommand
{
    static void Submit(const std::function<void(VkCommandBuffer)>& action)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = Context::Get().m_CommandPool->GetHandle();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(Context::Get().m_Device->GetHandle(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        action(commandBuffer);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(Context::Get().m_Device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(Context::Get().m_Device->GetGraphicsQueue());

        vkFreeCommandBuffers(Context::Get().m_Device->GetHandle(), Context::Get().m_CommandPool->GetHandle(), 1, &commandBuffer);
    }
}