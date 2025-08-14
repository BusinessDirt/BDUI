#pragma once

#include "Mixture/Core/Base.hpp"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace Mixture
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec4 Color;

        bool operator==(const Vertex& other) const
        {
            return Position == other.Position && Color == other.Color;
        }
    };

    struct FrameInfo
    {
        uint32_t FrameIndex;
        float FrameTime;
        VkCommandBuffer CommandBuffer;
        
        uint32_t TriangleCount;
        uint32_t DrawCalls;
    };
}
