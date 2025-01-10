#pragma once

#include "Platform/Vulkan/Base.hpp"

#include "Platform/Vulkan/Buffer/Buffer.hpp"

namespace Mixture::Vulkan
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const std::vector<uint32_t>& indices);
        ~IndexBuffer();

        void Bind(VkCommandBuffer commandBuffer);
        uint32_t GetIndexCount() { return m_IndexCount; }

    private:
        uint32_t m_IndexCount = 0;
        Scope<Buffer> m_IndexBuffer;
    };
}