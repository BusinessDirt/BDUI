#pragma once

#include "Mixture/Core/Base.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

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
        float FrameTime;
        VkCommandBuffer CommandBuffer;
    };
}
