#include "mxpch.hpp"
#include "MainLayer.hpp"

#include <imgui.h>

namespace Mixture
{
    void MainLayer::OnAttach()
    {
        const std::vector<Vertex> vertices = {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}
        };

        const std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };
    }

    void MainLayer::OnDetach()
    {
        
    }

    void MainLayer::OnUpdate(FrameInfo& frameInfo)
    {
    }

    void MainLayer::OnRenderUI(FrameInfo& frameInfo)
    {
        ImGui::Begin("Test");
        ImGui::Text("Frame time: %.3f ms", frameInfo.FrameTime);
        ImGui::End();
    }

    void MainLayer::OnEvent(Event& event)
    {
        
    }
}
