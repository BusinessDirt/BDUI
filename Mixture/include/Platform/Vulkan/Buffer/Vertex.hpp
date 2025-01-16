#pragma once

#include "Mixture/Renderer/RendererInfo.hpp"

#include "Platform/Vulkan/Base.hpp"
#include "Platform/Vulkan/Buffer/Buffer.hpp"

namespace Mixture
{
    namespace Vulkan
    {
        class VertexBuffer
        {
        public:
            VertexBuffer(const std::vector<Vertex>& vertices);
            ~VertexBuffer();

            void Bind(VkCommandBuffer commandBuffer);
            uint32_t GetVertexCount() { return m_VertexCount; }

        private:
            uint32_t m_VertexCount = 0;
            Scope<Buffer> m_VertexBuffer;
        };
    }
}
