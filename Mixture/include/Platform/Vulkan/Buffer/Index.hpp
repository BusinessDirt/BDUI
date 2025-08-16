#pragma once

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"

#include "Mixture/Renderer/RendererInfo.hpp"

namespace Mixture::Vulkan
{
    class IndexBuffer
    {
    public:
        IndexBuffer(const std::vector<uint32_t>& indices);
        IndexBuffer();
        ~IndexBuffer();

        void SetData(const std::vector<uint32_t>& indices, VkCommandBuffer commandBuffer = VK_NULL_HANDLE);
        
        void Bind(VkCommandBuffer commandBuffer);
        
        uint32_t GetIndexCount() { return m_IndexCount; }

    private:
        uint32_t m_IndexCount = 0;
        Scope<Buffer> m_IndexBuffer;
    };
}
