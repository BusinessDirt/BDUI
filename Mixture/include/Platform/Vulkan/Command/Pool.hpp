#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/PhysicalDevice.hpp"

namespace Mixture::Vulkan
{
    class CommandPool
    {
    public:
        OPAL_NON_COPIABLE(CommandPool);

        CommandPool();
        ~CommandPool();

    private:
        VULKAN_HANDLE(VkCommandPool, m_CommandPool);
    };
}