#pragma once

#include "Platform/Vulkan/Base.hpp"

namespace Vulkan
{
    class GraphicsPipeline
    {
    public:
        GraphicsPipeline();
        ~GraphicsPipeline();
        
        OPAL_NON_COPIABLE(GraphicsPipeline);
    };
}
