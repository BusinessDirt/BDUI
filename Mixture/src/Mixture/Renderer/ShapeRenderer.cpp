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
    }

    void ShapeRenderer::Shutdown()
    {
        m_VertexBuffer.reset();
        m_Pipeline.reset();
    }

    void ShapeRenderer::Begin()
    {
        m_Vertices.clear();
        
        const Window& window = Application::Get().GetWindow();
        m_PushConstant.ViewportSize = glm::vec2(window.GetWidth(), window.GetHeight());
    }

    void ShapeRenderer::AddRect(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
    {
        // We draw the rectangle as two triangles (6 vertices, no index buffer)
        glm::vec3 p0 = { start.x, start.y, 0 };
        glm::vec3 p1 = { end.x,   start.y, 0 };
        glm::vec3 p2 = { end.x,   end.y,   0 };
        glm::vec3 p3 = { start.x, end.y,   0 };

        // Triangle 1
        m_Vertices.push_back({ p0, color });
        m_Vertices.push_back({ p1, color });
        m_Vertices.push_back({ p2, color });

        // Triangle 2
        m_Vertices.push_back({ p0, color });
        m_Vertices.push_back({ p2, color });
        m_Vertices.push_back({ p3, color });
    }

    void ShapeRenderer::AddLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float thickness)
    {
        // Create a perpendicular offset for line thickness
        glm::vec2 dir = end - start;
        glm::vec2 perp = glm::normalize(glm::vec2(-dir.y, dir.x)) * (thickness * 0.5f);

        glm::vec2 v0 = start + perp;
        glm::vec2 v1 = start - perp;
        glm::vec2 v2 = end   - perp;
        glm::vec2 v3 = end   + perp;

        // Two triangles forming a quad
        m_Vertices.push_back({ { v0.x, v0.y, 0.0f }, color });
        m_Vertices.push_back({ { v1.x, v1.y, 0.0f }, color });
        m_Vertices.push_back({ { v2.x, v2.y, 0.0f }, color });

        m_Vertices.push_back({ { v0.x, v0.y, 0.0f }, color });
        m_Vertices.push_back({ { v2.x, v2.y, 0.0f }, color });
        m_Vertices.push_back({ { v3.x, v3.y, 0.0f }, color });
    }

    void ShapeRenderer::End()
    {
        
    }

    void ShapeRenderer::UploadBuffers(FrameInfo& frameInfo)
    {
        if (m_Vertices.size() <= 0) return;
        m_VertexBuffer->SetData(m_Vertices);
    }

    void ShapeRenderer::Render(FrameInfo& frameInfo)
    {
        if (m_Vertices.size() <= 0) return;
        m_Pipeline->Bind(frameInfo);
        m_Pipeline->PushConstants(frameInfo, &m_PushConstant);
        m_VertexBuffer->Bind(frameInfo);
        DrawCommand::Draw(frameInfo, m_VertexBuffer->GetVertexCount());
    }
}
