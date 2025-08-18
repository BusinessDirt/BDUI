#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/Shapes/Shapes.hpp"

#include "Platform/Vulkan/GraphicsPipeline.hpp"
#include "Platform/Vulkan/Buffer/Vertex.hpp"
#include "Platform/Vulkan/Buffer/Index.hpp"

#include <glm/glm.hpp>

namespace Mixture
{
    struct ShapeRendererPushConstant
    {
        glm::vec2 ViewportSize = { 1.0f, 1.0f };
    };

    class ShapeRenderer
    {
    public:
        ShapeRenderer() = default;
        
        void Initialize();
        void Shutdown();

        void AddRect(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
        void AddLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float thickness = 1.0f);

        void UploadBuffers();
        void Render(VkCommandBuffer commandBuffer);

    private:
        static const char* SHADER_NAME;
        
        ShapeRendererPushConstant m_PushConstant = {};
        
        Scope<Vulkan::GraphicsPipeline> m_Pipeline;
        Scope<Vulkan::VertexBuffer<Vertex>> m_VertexBuffer;
        Scope<Vulkan::IndexBuffer> m_IndexBuffer;

        std::vector<Scope<IShape>> m_Shapes;
        std::size_t m_ShapeHash;
    };
}
