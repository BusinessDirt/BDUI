#pragma once

#include "Mixture.hpp"

namespace Mixture
{
    class MainLayer : public Layer
    {
    public:
        MainLayer() : Layer("MainLayer") {}
        ~MainLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(FrameInfo& frameInfo) override;
        void OnRenderUI(FrameInfo& frameInfo) override;
        void OnEvent(Event& event) override;
        
    private:
        Scope<Vulkan::GraphicsPipeline> m_Pipeline;
        Scope<Vulkan::VertexBuffer> m_VertexBuffer;
        
        bool m_ViewportFocused = false;
    };
}

