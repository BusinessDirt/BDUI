#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Mixture::Vulkan
{
    class CommandBuffers
    {
    public:
        OPAL_NON_COPIABLE(CommandBuffers);

        CommandBuffers(const VkCommandPool commandPool, size_t size);
        ~CommandBuffers();

        VkCommandBuffer Get(int i) const { return m_CommandBuffers[i]; }
    private:
        std::vector<VkCommandBuffer> m_CommandBuffers;
        VkCommandPool m_CommandPool;
    };
}