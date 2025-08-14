#pragma once

#include "Mixture/Core/Base.hpp"
#include "Mixture/Renderer/RendererInfo.hpp"

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

        void Begin();
        void DrawRect(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
        void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float thickness = 1.0f);
        void End();

        void UploadBuffers(FrameInfo& frameInfo);
        void Render(FrameInfo& frameInfo);

    private:
        static const char* SHADER_NAME;
        
        ShapeRendererPushConstant m_PushConstant = {};
        
        Scope<Vulkan::GraphicsPipeline> m_Pipeline;
        Scope<Vulkan::VertexBuffer> m_VertexBuffer;
        Scope<Vulkan::IndexBuffer> m_IndexBuffer;

        std::vector<uint32_t> m_Indices;
        std::vector<Vertex> m_Vertices;
    };
}
