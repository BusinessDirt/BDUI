#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class CommandBuffers
    {
    public:
        OPAL_NON_COPIABLE(CommandBuffers);

        CommandBuffers(VkCommandPool commandPool, size_t size);
        ~CommandBuffers();

        VkCommandBuffer Get(const uint32_t index) const { return m_CommandBuffers[index]; }
    private:
        std::vector<VkCommandBuffer> m_CommandBuffers;
        VkCommandPool m_CommandPool;
    };
}