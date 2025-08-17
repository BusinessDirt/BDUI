#include "mxpch.hpp"
#include "Mixture/Renderer/Shapes/ShapeRenderer.hpp"
#include "Mixture/Renderer/DrawCommand.hpp"
#include "Mixture/Core/Application.hpp"

#include "Platform/Vulkan/Context.hpp"

namespace Mixture
{
    namespace Util
    {
        namespace
        {
            std::size_t HashShapeVector(const std::vector<Scope<IShape>>& shapes)
            {
                std::size_t seed = shapes.size();
                for (const auto& shape : shapes) shape->HashCombine(seed);
                return seed;
            }
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

    void ShapeRenderer::AddRect(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
    {
        m_Shapes.emplace_back(CreateScope<Rectangle>(glm::vec3(start.x, start.y, 0.0f),
                                                     glm::vec3(end.x, end.y, 0.0f),
                                                     color));
    }

    void ShapeRenderer::AddLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float thickness)
    {
        m_Shapes.emplace_back(CreateScope<Line>(glm::vec3(start.x, start.y, 0.0f),
                                                glm::vec3(end.x, end.y, 0.0f),
                                                color, thickness));
    }

    void ShapeRenderer::UploadBuffers()
    {
        // Set dirty flag and skip hashing if the size has changed
        std::size_t oldHash = m_ShapeHash;
        m_ShapeHash = Util::HashShapeVector(m_Shapes);
        if (m_ShapeHash == oldHash) return;
        
        // Tessellate shapes
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        for (const auto& shape : m_Shapes) shape->Tesselate(vertices, indices);
        
        if (vertices.empty() || indices.empty()) return;
        
        m_VertexBuffer->SetData(vertices);
        m_IndexBuffer->SetData(indices);
    }

    void ShapeRenderer::Render(const VkCommandBuffer commandBuffer)
    {
        if (m_Shapes.empty()) return;
        
        m_Pipeline->Bind(commandBuffer);
        m_Pipeline->PushConstants(commandBuffer, &m_PushConstant);
        
        m_VertexBuffer->Bind(commandBuffer);
        m_IndexBuffer->Bind(commandBuffer);
        
        DrawCommand::DrawIndexed(commandBuffer, m_IndexBuffer->GetIndexCount());
        
        // Reset for next frame
        m_Shapes.clear();
        
        const Window& window = Application::Get().GetWindow();
        m_PushConstant.ViewportSize = glm::vec2(window.GetWidth(), window.GetHeight());
    }
}
