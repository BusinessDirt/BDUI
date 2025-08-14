#include "mxpch.hpp"
#include "MainLayer.hpp"

#include <imgui.h>

namespace Mixture
{
    void MainLayer::OnAttach()
    {
        
    }

    void MainLayer::OnDetach()
    {
        
    }

    void MainLayer::OnUpdate(FrameInfo& frameInfo)
    {
        const Window& window = Application::Get().GetWindow();
        Renderer::Shapes().DrawRect({ 0.0f, 0.0f }, { 500.0f, 500.0f }, { 0.0f, 1.0f, 1.0f, 0.5f });
        Renderer::Shapes().DrawLine({ 0.0f, 0.0f }, { window.GetWidth(), window.GetHeight() }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
    }

    void MainLayer::OnRenderUI(FrameInfo& frameInfo)
    {
        ImGuiWidgets::Dockspace("MainDockspace", true, ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_PassthruCentralNode);
        //m_ViewportFocused = Renderer::DrawImGuiViewport();
        
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
