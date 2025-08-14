#include "mxpch.hpp"
#include "Mixture/Renderer/Shapes/ShapeRenderer.hpp"
#include "Mixture/Renderer/DrawCommand.hpp"
#include "Mixture/Core/Application.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture
{
    namespace Util
    {
        std::size_t HashShapeVector(const std::vector<Scope<IShape>>& shapes) {
            std::size_t seed = shapes.size();
            for (const auto& shape : shapes) shape->HashCombine(seed);
            return seed;
        }
    }

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
        m_Shapes.clear();
        m_Dirty = false;
        
        const Window& window = Application::Get().GetWindow();
        m_PushConstant.ViewportSize = glm::vec2(window.GetWidth(), window.GetHeight());
    }

    void ShapeRenderer::DrawRect(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
    {
        m_Shapes.emplace_back(CreateScope<Rectangle>(glm::vec3(start.x, start.y, 0.0f),
                                                     glm::vec3(end.x, end.y, 0.0f),
                                                     color));
    }

    void ShapeRenderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float thickness)
    {
        m_Shapes.emplace_back(CreateScope<Line>(glm::vec3(start.x, start.y, 0.0f),
                                                glm::vec3(end.x, end.y, 0.0f),
                                                color, thickness));
    }

    void ShapeRenderer::End()
    {
        // Set dirty flag and skip hashing if the size has changed
        if (m_ShapeSize != m_Shapes.size())
        {
            m_Dirty = true;
            m_ShapeSize = m_Shapes.size();
        }
        else
        {
            std::size_t oldHash = m_ShapeHash;
            m_ShapeHash = Util::HashShapeVector(m_Shapes);
            m_Dirty = m_ShapeHash != oldHash;
        }
        
        //if (!m_Dirty) return;
    }

    void ShapeRenderer::UploadBuffers(FrameInfo& frameInfo)
    {
        if (!m_Dirty) return;
        m_Dirty = false;
        
        // Tesselate shapes
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        for (const auto& shape : m_Shapes) shape->Tesselate(vertices, indices);
        
        if (vertices.size() == 0 || indices.size() == 0) return;
        
        m_VertexBuffer->SetData(vertices);
        m_IndexBuffer->SetData(indices);
    }

    void ShapeRenderer::Render(FrameInfo& frameInfo)
    {
        if (m_Shapes.size() <= 0) return;
        
        m_Pipeline->Bind(frameInfo);
        m_Pipeline->PushConstants(frameInfo, &m_PushConstant);
        
        m_VertexBuffer->Bind(frameInfo);
        m_IndexBuffer->Bind(frameInfo);
        
        DrawCommand::DrawIndexed(frameInfo, m_IndexBuffer->GetIndexCount());
    }
}
