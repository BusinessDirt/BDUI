#pragma once

#include "Mixture.hpp"

namespace Mixture
{
    struct UniformBufferObject
    {
        glm::mat4 View;
        glm::mat4 Projection;
    };

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
        Scope<Vulkan::GraphicsPipeline> m_GraphicsPipeline;
        Scope<Vulkan::VertexBuffer> m_VertexBuffer;
        Scope<Vulkan::IndexBuffer> m_IndexBuffer;
        
        Scope<Vulkan::Buffer> m_UniformBuffer;
        UniformBufferObject m_UniformBufferObject;
        float m_Z = 1.0f;
        
        bool m_ViewportFocused = false;
    };
}

