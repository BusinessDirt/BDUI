#pragma once

#include "Platform/Vulkan/Base.hpp"

#include "Platform/Vulkan/Buffer/Buffer.hpp"

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