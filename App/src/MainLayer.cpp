#include "mxpch.hpp"
#include "MainLayer.hpp"

#include <imgui.h>

namespace Mixture
{
    void MainLayer::OnAttach()
    {
        const glm::vec2 begin(-1.0f, -1.0f);
        const glm::vec2 end(0.50f, 0.50f);
        const glm::vec3 color(0.0f, 1.0f, 1.0f);
        
        static const std::vector<Vertex> vertices = {
            { { begin.x, begin.y, 0.0f }, color },
            { { end.x,   begin.y, 0.0f }, color },
            { { end.x,   end.y,   0.0f }, color },
            { { end.x,   end.y,   0.0f }, color },
            { { begin.x, end.y,   0.0f }, color },
            { { begin.x, begin.y, 0.0f }, color }
        };
        
        m_Pipeline = CreateScope<Vulkan::GraphicsPipeline>("shader");
        m_VertexBuffer = CreateScope<Vulkan::VertexBuffer>(vertices);
    }

    void MainLayer::OnDetach()
    {
        m_Pipeline.reset();
        m_VertexBuffer.reset();
    }

    void MainLayer::OnUpdate(FrameInfo& frameInfo)
    {
        m_Pipeline->Bind(frameInfo);
        m_VertexBuffer->Bind(frameInfo);
        DrawCommand::Draw(frameInfo, m_VertexBuffer->GetVertexCount());
    }

    void MainLayer::OnRenderUI(FrameInfo& frameInfo)
    {
        ImGuiWidgets::Dockspace("MainDockspace", true, ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_PassthruCentralNode);
        m_ViewportFocused = Renderer::DrawImGuiViewport();
        
        ImGui::Begin("Test");
        ImGui::Text("Frame time: %.3f ms", frameInfo.FrameTime);
        ImGui::Text("Viewport focused: %s", m_ViewportFocused ? "true" : "false");
        ImGui::Text("Triangle Count: %d", frameInfo.TriangleCount);
        ImGui::Text("Draw Calls: %d", frameInfo.DrawCalls);
        ImGui::End();
    }

    void MainLayer::OnEvent(Event& event)
    {
        
    }
}
