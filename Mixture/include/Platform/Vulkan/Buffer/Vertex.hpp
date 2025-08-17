#pragma once

#include "Mixture/Renderer/RendererInfo.hpp"

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"

namespace Mixture::Vulkan
{
    class VertexBuffer
    {
    public:
        OPAL_NON_COPIABLE(VertexBuffer);

        explicit VertexBuffer(const std::vector<Vertex>& vertices);
        VertexBuffer(const void* vertices, uint32_t vertexCount, uint32_t vertexSize);
        VertexBuffer();
        ~VertexBuffer();

        void SetData(const std::vector<Vertex>& vertices, VkCommandBuffer commandBuffer = VK_NULL_HANDLE);
        void SetData(const void* vertices, uint32_t vertexCount, uint32_t vertexSize, VkCommandBuffer commandBuffer = VK_NULL_HANDLE);
            
        void Bind(VkCommandBuffer commandBuffer) const;
            
        OPAL_NODISCARD uint32_t GetVertexCount() const { return m_VertexCount; }

    private:
        uint32_t m_VertexCount = 0;
        Scope<Buffer> m_VertexBuffer;
    };
}
