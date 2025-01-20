#include "mxpch.hpp"
#include "MainLayer.hpp"

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
        ShapeRenderer::DrawRectangle({ -1.0f, -1.0f }, { 0.50f, 0.50f }, { 0.0f, 1.0f, 1.0f });
        ShapeRenderer::DrawRectangle({ -0.75f, -0.75f }, { 0.75f, 0.75f }, { 1.0f, 1.0f, 1.0f });
    }

    void MainLayer::OnEvent(Event& event)
    {
        
    }
}
