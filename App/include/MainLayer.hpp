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
        void OnRender(FrameInfo& frameInfo) override;
        void OnRenderImGui(FrameInfo& frameInfo) override;
        
        void OnEvent(Event& event) override;
        
    private:
        bool m_ViewportFocused = false;
    };
}

