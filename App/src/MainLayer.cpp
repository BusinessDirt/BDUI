#include "mxpch.hpp"
#include "MainLayer.hpp"

namespace Mixture
{
    void MainLayer::OnAttach()
    {
        m_Pipeline = CreateScope<Vulkan::GraphicsPipeline>();

        const std::vector<Vertex> vertices = {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}
        };

        const std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        m_VertexBuffer = CreateScope<Vulkan::VertexBuffer>(vertices);
        m_IndexBuffer = CreateScope<Vulkan::IndexBuffer>(indices);
    }

    void MainLayer::OnDetach()
    {
        m_IndexBuffer = nullptr;
        m_VertexBuffer = nullptr;
        m_Pipeline = nullptr;
    }

    void MainLayer::OnUpdate(FrameInfo& frameInfo)
    {
        m_Pipeline->Bind(frameInfo);
        m_VertexBuffer->Bind(frameInfo.CommandBuffer);
        m_IndexBuffer->Bind(frameInfo.CommandBuffer);
        DrawCommand::DrawIndexed(frameInfo.CommandBuffer, m_IndexBuffer->GetIndexCount());
    }

    void MainLayer::OnEvent(Event& event)
    {
        
    }
}