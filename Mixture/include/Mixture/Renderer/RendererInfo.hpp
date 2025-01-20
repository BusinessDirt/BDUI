#pragma once

#include "Mixture/Core/Base.hpp"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace Mixture
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Color;

        bool operator==(const Vertex& other) const
        {
            return Position == other.Position && Color == other.Color;
        }
    };

    struct VkDrawIndexedIndirectCommand 
    {
        uint32_t    IndexCount;
        uint32_t    InstanceCount;
        uint32_t    FirstIndex;
        int32_t     VertexOffset;
        uint32_t    FirstInstance;
    };

    struct VkDrawIndirectCommand 
    {
        uint32_t    VertexCount;
        uint32_t    InstanceCount;
        uint32_t    FirstVertex;
        uint32_t    FirstInstance;
    };

    struct FrameInfo
    {
        uint32_t FrameIndex;
        float FrameTime;
        VkCommandBuffer CommandBuffer;
    };
}
