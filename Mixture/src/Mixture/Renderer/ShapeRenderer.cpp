#include "mxpch.hpp"
#include "Mixture/Renderer/ShapeRenderer.hpp"
#include "Mixture/Renderer/DrawCommand.hpp"
#include "Mixture/Core/Application.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture
{
    const char* ShapeRenderer::SHADER_NAME = "Mixture_ShapeRenderer";

    void ShapeRenderer::Initialize()
    {
        m_Pipeline = CreateScope<Vulkan::GraphicsPipeline>(SHADER_NAME);
        m_VertexBuffer = CreateScope<Vulkan::VertexBuffer>();
        m_IndexBuffer = CreateScope<Vulkan::IndexBuffer>();
    }

    void ShapeRenderer::Shutdown()
    {
        m_IndexBuffer.reset();
        m_VertexBuffer.reset();
        m_Pipeline.reset();
    }

    void ShapeRenderer::Begin()
    {
        m_Vertices.clear();
        m_Indices.clear();
        
        const Window& window = Application::Get().GetWindow();
        m_PushConstant.ViewportSize = glm::vec2(window.GetWidth(), window.GetHeight());
    }

    void ShapeRenderer::DrawRect(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
    {
        uint32_t startIndex = static_cast<uint32_t>(m_Vertices.size());

        m_Vertices.push_back({ { start.x, start.y, 0.0f }, color });
        m_Vertices.push_back({ { end.x,   start.y, 0.0f }, color });
        m_Vertices.push_back({ { end.x,   end.y,   0.0f }, color });
        m_Vertices.push_back({ { start.x, end.y,   0.0f }, color });

        m_Indices.insert(m_Indices.end(), {
            startIndex, startIndex+1, startIndex+2,
            startIndex, startIndex+2, startIndex+3
        });
    }

    void ShapeRenderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float thickness)
    {
        // Create a perpendicular offset for line thickness
        glm::vec2 dir = end - start;
        glm::vec2 perp = glm::normalize(glm::vec2(-dir.y, dir.x)) * (thickness * 0.5f);

        glm::vec2 v0 = start + perp;
        glm::vec2 v1 = start - perp;
        glm::vec2 v2 = end   - perp;
        glm::vec2 v3 = end   + perp;

        uint32_t startIndex = static_cast<uint32_t>(m_Vertices.size());
        
        m_Vertices.push_back({ { v0.x, v0.y, 0.0f }, color });
        m_Vertices.push_back({ { v1.x, v1.y, 0.0f }, color });
        m_Vertices.push_back({ { v2.x, v2.y, 0.0f }, color });
        m_Vertices.push_back({ { v3.x, v3.y, 0.0f }, color });
        
        m_Indices.insert(m_Indices.end(), {
            startIndex, startIndex+1, startIndex+2,
            startIndex, startIndex+2, startIndex+3
        });
    }

    void ShapeRenderer::End()
    {
        
    }

    void ShapeRenderer::UploadBuffers(FrameInfo& frameInfo)
    {
        if (m_Vertices.size() <= 0) return;
        m_VertexBuffer->SetData(m_Vertices);
        m_IndexBuffer->SetData(m_Indices);
    }

    void ShapeRenderer::Render(FrameInfo& frameInfo)
    {
        if (m_Vertices.size() <= 0) return;
        
        m_Pipeline->Bind(frameInfo);
        m_Pipeline->PushConstants(frameInfo, &m_PushConstant);
        
        m_VertexBuffer->Bind(frameInfo);
        m_IndexBuffer->Bind(frameInfo);
        
        DrawCommand::DrawIndexed(frameInfo, m_IndexBuffer->GetIndexCount());
    }
}
