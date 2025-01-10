#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Vulkan
{
    class GraphicsPipeline
    {
    public:
        GraphicsPipeline(const VkDevice device);
        ~GraphicsPipeline();
        
        OPAL_NON_COPIABLE(GraphicsPipeline);

    private:

        const VkDevice m_Device;
    };
}
