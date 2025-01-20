#pragma once

#include "Mixture/Core/Base.hpp"

#include "Platform/Vulkan/Buffer/Index.hpp"
#include "Platform/Vulkan/Buffer/Vertex.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"
#include "Platform/Vulkan/GraphicsPipeline.hpp"

namespace Mixture
{
    static Scope<Vulkan::IndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& indices)
    {
        return CreateScope<Vulkan::IndexBuffer>(indices);
    }

    static Scope<Vulkan::VertexBuffer> CreateVertexBuffer(const std::vector<Vertex>& vertices)
    {
        return CreateScope<Vulkan::VertexBuffer>(vertices);
    }

    static Scope<Vulkan::VertexBuffer> CreateVertexBuffer()
    {
        return CreateScope<Vulkan::VertexBuffer>();
    }

    static Scope<Vulkan::Buffer> CreateBuffer(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1)
    {
        return CreateScope<Vulkan::Buffer>(instanceSize, instanceCount, usageFlags, memoryPropertyFlags, minOffsetAlignment);
    }

    static Scope<Vulkan::GraphicsPipeline> CreateGraphicsPipeline(const std::string& shaderName)
    {
        return CreateScope<Vulkan::GraphicsPipeline>(shaderName);
    }
}
