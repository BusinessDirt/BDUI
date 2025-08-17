#include "mxpch.hpp"
#include "Mixture/Renderer/LayerStack.hpp"

namespace Mixture
{
    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::Update(FrameInfo& frameInfo) const
    {
        for (Layer* layer : m_Layers) layer->OnUpdate(frameInfo);
    }

    void LayerStack::Render(FrameInfo& frameInfo) const
    {
        for (Layer* layer : m_Layers) layer->OnRender(frameInfo);
    }

    void LayerStack::RenderImGui(FrameInfo& frameInfo) const
    {
        for (Layer* layer : m_Layers) layer->OnRenderImGui(frameInfo);
    }

    void LayerStack::OnEvent(Event& e) const
    {
        for (Layer* layer : m_Layers) layer->OnEvent(e);
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_Layers.emplace_back(layer);
        layer->OnAttach();
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        if (const auto it = std::ranges::find(m_Layers, layer); it != m_Layers.end())
        {
            layer->OnDetach();
            m_Layers.erase(it);
        }
    }
}
