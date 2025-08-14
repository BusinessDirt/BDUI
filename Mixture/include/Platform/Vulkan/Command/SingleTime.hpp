#pragma once

#include "Platform/Vulkan/Base.hpp"

#include "Platform/Vulkan/Device.hpp"
#include "Platform/Vulkan/Context.hpp"
#include "Platform/Vulkan/Command/Pool.hpp"

namespace Mixture::Vulkan::SingleTimeCommand
{
    static VkCommandBuffer Begin()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = Context::Get().CommandPool().GetHandle();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(Context::Get().Device().GetHandle(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        return commandBuffer;
    }

    static void End(VkCommandBuffer commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(Context::Get().Device().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(Context::Get().Device().GetGraphicsQueue());

        vkFreeCommandBuffers(Context::Get().Device().GetHandle(), Context::Get().CommandPool().GetHandle(), 1, &commandBuffer);
    }

    static void Submit(const std::function<void(VkCommandBuffer)>& action)
    {
        VkCommandBuffer commandBuffer = Begin();
        action(commandBuffer);
        End(commandBuffer);
    }
}
